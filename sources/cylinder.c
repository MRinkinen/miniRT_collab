/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:20:42 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 16:23:21 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

void	cylinder_helper(t_object *object, t_tuple center, \
t_tuple orientation, float radius)
{
	t_matrix	*temp;

	object->type = CYLINDER;
	object->data.cylinder.translation_matrix = \
	translation(center.x, center.y, center.z);
	object->data.cylinder.rotation_matrix = rotation_from_normal(orientation);
	object->data.cylinder.scaling_matrix = scaling(radius, 1.0f, radius);
	temp = t_matrix_multiply(object->data.cylinder.scaling_matrix, \
	object->data.cylinder.rotation_matrix);
	object->data.cylinder.transform = t_matrix_multiply(temp, \
	object->data.cylinder.translation_matrix);
	free(temp);
	object->data.cylinder.inverse_transform = \
	inverse(object->data.cylinder.transform);
}

void	create_cylinders(t_var *var, t_map *map, int *obj_index)
{
	t_cylinders	*current_cylinder;
	t_object	*object;
	t_tuple		orientation;
	t_tuple		center;
	float		radius;

	current_cylinder = map->cylinders;
	while (current_cylinder != NULL)
	{
		object = &var->objects[*obj_index];
		orientation = normalize(vector(current_cylinder->nx, \
		current_cylinder->ny, current_cylinder->nz));
		center = point(current_cylinder->x, current_cylinder->y, \
		current_cylinder->z);
		radius = current_cylinder->diameter / 2.0f;
		object->data.cylinder.center = center;
		object->data.cylinder.radius = radius;
		object->data.cylinder.height = current_cylinder->height;
		object->data.cylinder.orientation = orientation;
		object->data.cylinder.color = t_color_create(current_cylinder->r, \
		current_cylinder->g, current_cylinder->b);
		cylinder_helper(object, center, orientation, radius);
		current_cylinder = current_cylinder->next;
		(*obj_index)++;
	}
}


t_tuple	calculate_cylinder_normal(t_cylinder *cylinder, t_tuple *point)
{
	t_tuple	btm_cent;
	t_tuple	top_cent;
	t_tuple	surf_normal;
	double	t;
	t_tuple	local_point;

	cylinder->orientation = normalize(cylinder->orientation);
	btm_cent = tuple_subtract(cylinder->center, \
	tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	top_cent = tuple_add(cylinder->center, \
	tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	if (magnitude(tuple_subtract(*point, top_cent)) < cylinder->radius)
		return (cylinder->orientation);
	if (magnitude(tuple_subtract(*point, btm_cent)) < cylinder->radius)
		return (tuple_multiply(cylinder->orientation, -1));
	t = dot(tuple_subtract(*point, btm_cent), cylinder->orientation);
	local_point = tuple_add(btm_cent, tuple_multiply(cylinder->orientation, t));
	surf_normal = normalize(tuple_subtract(*point, local_point));
	return (surf_normal);
}

void	set_cylinder_params(const t_ray *ray, const t_cylinder *cylinder, \
t_cylinder_params *params)
{
	params->oc = tuple_subtract(cylinder->center, ray->origin);
	params->direction = ray->direction;
	params->axis = cylinder->orientation;
	params->radius = cylinder->radius;
}

float	calculate_discriminant(t_cylinder_params *params)
{
	params->a = dot(params->direction, params->direction) - \
	pow(dot(params->direction, params->axis), 2);
	params->b = dot(params->direction, params->oc) - \
	dot(params->direction, params->axis) * dot(params->oc, params->axis);
	params->c = dot(params->oc, params->oc) - \
	pow(dot(params->oc, params->axis), 2) - params->radius * params->radius;
	return (params->b * params->b) - (params->a * params->c);
}

bool	check_cylinder_height(const t_ray *ray, const t_cylinder *cylinder, \
float *t, float t0, float t1)
{
	float	half_height;
	t_tuple	point;
	float	dist;

	half_height = cylinder->height / 2.0;
	point = tuple_add(ray->origin, tuple_multiply(ray->direction, t0));
	dist = dot(point, cylinder->orientation) - \
	dot(cylinder->center, cylinder->orientation);
	if (dist >= -half_height && dist <= half_height)
	{
		*t = t0;
		return (true);
	}
	point = tuple_add(ray->origin, tuple_multiply(ray->direction, t1));
	dist = dot(point, cylinder->orientation) - \
	dot(cylinder->center, cylinder->orientation);
	if (dist >= -half_height && dist <= half_height)
	{
		*t = t1;
		return (true);
	}
	return (false);
}

bool intersect_cylinder_body(const t_ray *ray, const t_cylinder \
*cylinder, float *t)
{
	t_cylinder_params	params;
	float				discriminant;
	float				t0;
	float				t1;
	bool				hit;

	set_cylinder_params(ray, cylinder, &params);
	discriminant = calculate_discriminant(&params);
	hit = false;
	if (discriminant >= 0)
	{
		t0 = (params.b - sqrt(discriminant)) / params.a;
		t1 = (params.b + sqrt(discriminant)) / params.a;
		hit = check_cylinder_height(ray, cylinder, t, t0, t1);
	}
	return (hit);
}

bool intersect_cylinder_caps(const t_ray *ray, const \
t_cylinder *cylinder, float *t, bool hit)
{
	t_tuple	cap_center;
	t_tuple	cap_point;
	float	t_cap;
	float	cap_dist;

	cap_center = tuple_add(cylinder->center, \
	tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	t_cap = (dot(cap_center, cylinder->orientation) - dot(ray->origin, \
	cylinder->orientation)) / dot(ray->direction, cylinder->orientation);
	cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
	cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), \
	tuple_subtract(cap_point, cap_center)));
	if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t))
	{
		*t = t_cap;
		hit = true;
	}
	cap_center = tuple_subtract(cylinder->center, \
	tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	t_cap = (dot(cap_center, cylinder->orientation) - dot(ray->origin, \
	cylinder->orientation)) / dot(ray->direction, cylinder->orientation);
	cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
	cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), \
	tuple_subtract(cap_point, cap_center)));
	if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t))
	{
		*t = t_cap;
		hit = true;
	}
	return (hit);
}

bool intersect_cylinder(const t_ray *ray, \
const t_cylinder *cylinder, float *t)
{
	t_ray	transformed_ray;
	bool	hit;

	transformed_ray.origin = \
	apply_transformation(cylinder->inverse_transform, &ray->origin);
	transformed_ray.direction = \
	apply_transformation(cylinder->inverse_transform, &ray->direction);

	hit = intersect_cylinder_body(ray, cylinder, t);
	hit = intersect_cylinder_caps(ray, cylinder, t, hit) || hit;
	return (hit);
}

void fill_inverse(t_matrix *inverse_m, t_matrix *adjugate_m, float det)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < inverse_m->rows)
	{
		j = 0;
		while (j < inverse_m->cols)
		{
			inverse_m->data[i][j] = adjugate_m->data[i][j] / det;
			j++;
		}
		i++;
	}
}

t_matrix *inverse(t_matrix *m)
{
	float		det;
	t_matrix	*cofactor_m;
	t_matrix	*adjugate_m;
	t_matrix	*inverse_m;

	det = determinant(m);
	if (det == 0.0f)
	{
		printf("Matrix is not invertible.\n");
		exit(1);
	}
	cofactor_m = cofactor_matrix(m);
	adjugate_m = transpose(cofactor_m);
	inverse_m = (t_matrix *)malloc(sizeof(t_matrix));
	inverse_m->rows = m->rows;
	inverse_m->cols = m->cols;
	fill_inverse(inverse_m, adjugate_m, det);
	free(cofactor_m);
	free(adjugate_m);
	return (inverse_m);
}

t_matrix *cofactor_matrix(const t_matrix *m)
{
	int			i;
	int			j;
	t_matrix	*cofactor_m;

	cofactor_m = (t_matrix *)malloc(sizeof(t_matrix));
	if (cofactor_m == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	cofactor_m->rows = m->rows;
	cofactor_m->cols = m->cols;
	i = 0;
	while (i < m->rows)
	{
		j = 0;
		while (j < m->cols)
		{
			cofactor_m->data[i][j] = cofactor(m, i, j);
			j++;
		}
		i++;
	}
	return (cofactor_m);
}

t_tuple	get_default_normal(void)
{
	t_tuple	default_normal;

	default_normal = vector(0, 1, 0);
	return (normalize(default_normal));
}

bool	check_special_case(t_tuple v2)
{
	if (fabs(v2.x - 1.0) < EPSILON && fabs(v2.y) < \
	EPSILON && fabs(v2.z) < EPSILON)
		return (true);
	if (fabs(v2.z - 1.0) < EPSILON && fabs(v2.x) < \
	EPSILON && fabs(v2.y) < EPSILON)
		return (true);
	return (false);
}

t_tuple get_arbitrary_axis(t_tuple v1)
{
	t_tuple	arbitrary_axis;

	arbitrary_axis = vector(1, 0, 0);
	if (fabs(v1.x) > fabs(v1.y))
		arbitrary_axis = vector(0, 1, 0);
	return (arbitrary_axis);
}

float minor(const t_matrix *m, int row, int col)
{
	t_matrix *sub_m;
	float det;

	sub_m = submatrix(m, row, col);
	det = determinant(sub_m);
	free(sub_m);
	return (det);
}

float cofactor(const t_matrix *m, int row, int col)
{
	float	minor_value;

	minor_value = minor(m, row, col);
	if ((row + col) % 2 != 0)
	{
		return (-minor_value);
	}
	return (minor_value);
}

t_tuple matrix_to_tuple(t_matrix *m)
{
	return (tuple(m->data[0][0], m->data[1][0], m->data[2][0], m->data[3][0]));
}

t_matrix	*translation(float x, float y, float z)
{
	t_matrix	*transform;

	transform = identity_matrix();
	transform->data[0][3] = x;
	transform->data[1][3] = y;
	transform->data[2][3] = z;

	return (transform);
}

t_matrix *allocate_submatrix(int new_rows, int new_cols)
{
	t_matrix	*sub_m;

	sub_m = (t_matrix *)malloc(sizeof(t_matrix));
	if (sub_m == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	sub_m->rows = new_rows;
	sub_m->cols = new_cols;
	return (sub_m);
}

void fill_submatrix(t_matrix *sub_m, const t_matrix *m, \
int remove_row, int remove_col)
{
	int	i;
	int	j;
	int	row_offset;
	int	col_offset;

	i = 0;
	while (i < sub_m->rows)
	{
		if (i >= remove_row)
			row_offset = 1;
		else
			row_offset = 0;
		j = 0;
		while (j < sub_m->cols)
		{
			if (j >= remove_col)
				col_offset = 1;
			else
				col_offset = 0;

			sub_m->data[i][j] = m->data[i + row_offset][j + col_offset];
			j++;
		}
		i++;
	}
}

t_matrix *submatrix(const t_matrix *m, int remove_row, int remove_col)
{
	int			new_rows;
	int			new_cols;
	t_matrix	*sub_m;

	new_rows = m->rows - 1;
	new_cols = m->cols - 1;
	sub_m = allocate_submatrix(new_rows, new_cols);
	if (sub_m == NULL)
		return (NULL);
	fill_submatrix(sub_m, m, remove_row, remove_col);
	return (sub_m);
}

t_matrix *allocate_matrix(int rows, int cols)
{
	t_matrix	*m;

	m = (t_matrix *)malloc(sizeof(t_matrix));
	if (m == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	m->rows = rows;
	m->cols = cols;
	return (m);
}

void initialize_identity(t_matrix *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->rows)
	{
		j = 0;
		while (j < m->cols)
		{
			if (i == j)
				m->data[i][j] = 1.0f;
			else
				m->data[i][j] = 0.0f;
			j++;
		}
		i++;
	}
}

t_matrix *scaling(float x, float y, float z)
{
	t_matrix	*transform;

	transform = allocate_matrix(4, 4);
	if (transform == NULL)
		return (NULL);
	initialize_identity(transform);
	transform->data[0][0] = x;
	transform->data[1][1] = y;
	transform->data[2][2] = z;
	return (transform);
}

t_matrix *tuple_to_matrix(t_tuple *t)
{
	float		values[16];
	t_matrix	*m;

	values[0] = t->x;
	values[1] = 0;
	values[2] = 0;
	values[3] = 0;
	values[4] = t->y;
	values[5] = 0;
	values[6] = 0;
	values[7] = 0;
	values[8] = t->z;
	values[9] = 0;
	values[10] = 0;
	values[11] = 0;
	values[12] = t->w;
	values[13] = 0;
	values[14] = 0;
	values[15] = 0;
	m = create_4x4_matrix(values);
	return (m);
}

t_tuple apply_transformation(t_matrix *transformation, t_tuple *point)
{
	t_matrix	*point_matrix;
	t_matrix	*transformed_matrix;
	t_tuple		transformed_point;

	point_matrix = tuple_to_matrix(point);
	transformed_matrix = t_matrix_multiply(transformation, point_matrix);
	transformed_point = matrix_to_tuple(transformed_matrix);
	free(point_matrix);
	free(transformed_matrix);
	return (transformed_point);
}

void	fill_rotation_values(float *values, t_tuple axis, \
t_rotation_params params)
{
	float	x;
	float	y;
	float	z;

	x = axis.x;
	y = axis.y;
	z = axis.z;
	values[0] = params.cos_theta + x * x * params.one_minus_cos;
	values[1] = x * y * params.one_minus_cos - z * params.sin_theta;
	values[2] = x * z * params.one_minus_cos + y * params.sin_theta;
	values[3] = 0;
	values[4] = y * x * params.one_minus_cos + z * params.sin_theta;
	values[5] = params.cos_theta + y * y * params.one_minus_cos;
	values[6] = y * z * params.one_minus_cos - x * params.sin_theta;
	values[7] = 0;
	values[8] = z * x * params.one_minus_cos - y * params.sin_theta;
	values[9] = z * y * params.one_minus_cos + x * params.sin_theta;
	values[10] = params.cos_theta + z * z * params.one_minus_cos;
	values[11] = 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;
	values[15] = 1;
}

t_matrix	*rotation_around_axis(t_tuple axis, float angle)
{
	t_matrix			*rotation;
	t_rotation_params	params;
	float				values[16];

	params.cos_theta = cos(angle);
	params.sin_theta = sin(angle);
	params.one_minus_cos = 1.0f - params.cos_theta;

	fill_rotation_values(values, axis, params);

	rotation = create_4x4_matrix(values);
	return (rotation);
}

t_matrix	*handle_axis_length_case(t_tuple v1, t_tuple v2)
{
	t_tuple	axis;
	t_tuple	arbitrary_axis;

	if (dot(v1, v2) > 0)
		return (identity_matrix());
	arbitrary_axis = get_arbitrary_axis(v1);
	axis = normalize(cross(v2, arbitrary_axis));
	return (rotation_around_axis(axis, PI));
}

t_matrix	*handle_special_case(t_tuple v2)
{
	if (fabs(v2.x - 1.0) < EPSILON)
	{
		return (rotation_around_axis(vector(0, 0, 1), PI / 2));
	}
	return (identity_matrix());
}

t_matrix	*rotation_from_normal(t_tuple normal)
{
	t_tuple	v1;
	t_tuple	v2;
	t_tuple	axis;
	float	axis_length;
	float	angle;

	v1 = get_default_normal();
	v2 = normalize(normal);
	if (check_special_case(v2))
	{
		return (handle_special_case(v2));
	}
	axis = cross(v2, v1);
	axis_length = magnitude(axis);
	if (axis_length < EPSILON)
	{
		return (handle_axis_length_case(v1, v2));
	}
	axis = normalize(axis);
	angle = acos(dot(v1, v2));
	return (rotation_around_axis(axis, angle));
}
