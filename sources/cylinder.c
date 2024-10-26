/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:20:42 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 03:37:20 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

typedef struct s_cylinder_params
{
    t_tuple oc;
    t_tuple direction;
    t_tuple axis;
    float radius;
    float a;
    float b;
    float c;
} t_cylinder_params;

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

/*
void create_cylinders(t_var *var, t_map *map, int *obj_index)
{
	t_cylinders *current_cylinder;
	t_tuple orientation;
	t_tuple center;
	float radius;
	float height;
	t_color color;
	t_matrix *temp;

	current_cylinder = map->cylinders;
	while (current_cylinder != NULL)
	{
		orientation = normalize(vector(current_cylinder->nx, current_cylinder->ny, current_cylinder->nz));
		center = point(current_cylinder->x, current_cylinder->y, current_cylinder->z);
		radius = current_cylinder->diameter / 2.0f;
		height = current_cylinder->height;
		color = t_color_create(current_cylinder->r, current_cylinder->g, current_cylinder->b);
		var->objects[*obj_index].data.cylinder.translation_matrix = translation(center.x, center.y, center.z);
		var->objects[*obj_index].data.cylinder.rotation_matrix = rotation_from_normal(orientation);
		var->objects[*obj_index].data.cylinder.scaling_matrix = scaling(radius, 1.0f, radius);
		temp = t_matrix_multiply(var->objects[*obj_index].data.cylinder.scaling_matrix, var->objects[*obj_index].data.cylinder.rotation_matrix);
		var->objects[*obj_index].data.cylinder.transform = t_matrix_multiply(temp, var->objects[*obj_index].data.cylinder.translation_matrix);
		free(temp);
		var->objects[*obj_index].data.cylinder.inverse_transform = inverse(var->objects[*obj_index].data.cylinder.transform);
		var->objects[*obj_index].type = CYLINDER;
		var->objects[*obj_index].data.cylinder.center = center;
		var->objects[*obj_index].data.cylinder.radius = radius;
		var->objects[*obj_index].data.cylinder.height = height;
		var->objects[*obj_index].data.cylinder.color = color;
		var->objects[*obj_index].data.cylinder.orientation = orientation;
		current_cylinder = current_cylinder->next;
		(*obj_index)++;
	}
}*/

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

void set_cylinder_params(const t_ray *ray, const t_cylinder *cylinder, t_cylinder_params *params)
{
    params->oc = tuple_subtract(cylinder->center, ray->origin);
    params->direction = ray->direction;
    params->axis = cylinder->orientation;
    params->radius = cylinder->radius;
}

float calculate_discriminant(t_cylinder_params *params)
{
    params->a = dot(params->direction, params->direction) - pow(dot(params->direction, params->axis), 2);
    params->b = dot(params->direction, params->oc) - dot(params->direction, params->axis) * dot(params->oc, params->axis);
    params->c = dot(params->oc, params->oc) - pow(dot(params->oc, params->axis), 2) - params->radius * params->radius;
    return (params->b * params->b) - (params->a * params->c);
}

bool check_cylinder_height(const t_ray *ray, const t_cylinder *cylinder, float *t, float t0, float t1)
{
    float half_height;
    t_tuple point;
    float dist;

    half_height = cylinder->height / 2.0;
    point = tuple_add(ray->origin, tuple_multiply(ray->direction, t0));
    dist = dot(point, cylinder->orientation) - dot(cylinder->center, cylinder->orientation);
    if (dist >= -half_height && dist <= half_height)
    {
        *t = t0;
        return true;
    }
    point = tuple_add(ray->origin, tuple_multiply(ray->direction, t1));
    dist = dot(point, cylinder->orientation) - dot(cylinder->center, cylinder->orientation);
    if (dist >= -half_height && dist <= half_height)
    {
        *t = t1;
        return true;
    }
    return false;
}

bool intersect_cylinder_body(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    t_cylinder_params params;
    float discriminant;
    float t0;
    float t1;
    bool hit;

    set_cylinder_params(ray, cylinder, &params);
    discriminant = calculate_discriminant(&params);
    hit = false;
    if (discriminant >= 0)
    {
        t0 = (params.b - sqrt(discriminant)) / params.a;
        t1 = (params.b + sqrt(discriminant)) / params.a;
        hit = check_cylinder_height(ray, cylinder, t, t0, t1);
    }
    return hit;
}

bool intersect_cylinder_caps(const t_ray *ray, const t_cylinder *cylinder, float *t, bool hit)
{
	t_tuple cap_center;
	t_tuple cap_point;
	float t_cap;
	float cap_dist;

	cap_center = tuple_add(cylinder->center, tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	t_cap = (dot(cap_center, cylinder->orientation) - dot(ray->origin, cylinder->orientation)) / dot(ray->direction, cylinder->orientation);
	cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
	cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), tuple_subtract(cap_point, cap_center)));
	if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t))
	{
		*t = t_cap;
		hit = true;
	}
	cap_center = tuple_subtract(cylinder->center, tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	t_cap = (dot(cap_center, cylinder->orientation) - dot(ray->origin, cylinder->orientation)) / dot(ray->direction, cylinder->orientation);
	cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
	cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), tuple_subtract(cap_point, cap_center)));
	if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t))
	{
		*t = t_cap;
		hit = true;
	}
	return hit;
}

bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
	t_ray transformed_ray;
	bool hit;

	transformed_ray.origin = apply_transformation(cylinder->inverse_transform, &ray->origin);
	transformed_ray.direction = apply_transformation(cylinder->inverse_transform, &ray->direction);

	hit = intersect_cylinder_body(ray, cylinder, t);
	hit = intersect_cylinder_caps(ray, cylinder, t, hit) || hit;
	return hit;
}

/*
bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
	t_ray transformed_ray;
	transformed_ray.origin = apply_transformation(cylinder->inverse_transform, &ray->origin);
	transformed_ray.direction = apply_transformation(cylinder->inverse_transform, &ray->direction);
	t_tuple oc = tuple_subtract(cylinder->center, ray->origin);
	t_tuple direction = ray->direction;
	t_tuple axis = cylinder->orientation;
	float a = dot(direction, direction) - pow(dot(direction, axis), 2);
	float b = (dot(direction, oc) - dot(direction, axis) * dot(oc, axis));
	float c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;
	float discriminant = (b * b) - (a * c);

	float t0, t1;
	bool hit = false;
	if (discriminant >= 0) {
		t0 = (b - sqrt(discriminant)) / a;
		t1 = (b + sqrt(discriminant)) / a;

		if (t0 > t1) {
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}
		float half_height = cylinder->height / 2.0;
		t_tuple point0 = tuple_add(ray->origin, tuple_multiply(ray->direction, t0));
		t_tuple point1 = tuple_add(ray->origin, tuple_multiply(ray->direction, t1));
		float dist0 = dot(point0, axis) - dot(cylinder->center, axis);
		float dist1 = dot(point1, axis) - dot(cylinder->center, axis);
		if (dist0 >= -half_height && dist0 <= half_height) {
			*t = t0;
			hit = true;
		} else if (dist1 >= -half_height && dist1 <= half_height) {
			*t = t1;
			hit = true;
		}
	}
	// Check for intersection with the caps
	float t_cap;
	t_tuple cap_center;
	t_tuple cap_point;
	float cap_dist;
	// Top cap
	cap_center = tuple_add(cylinder->center, tuple_multiply(axis, cylinder->height / 2.0));
	t_cap = (dot(cap_center, axis) - dot(ray->origin, axis)) / dot(ray->direction, axis);
	cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
	cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), tuple_subtract(cap_point, cap_center)));
	if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t)) {
		*t = t_cap;
		hit = true;
	}
	// Bottom cap
	cap_center = tuple_subtract(cylinder->center, tuple_multiply(axis, cylinder->height / 2.0));
	t_cap = (dot(cap_center, axis) - dot(ray->origin, axis)) / dot(ray->direction, axis);
	cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
	cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), tuple_subtract(cap_point, cap_center)));
	if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t)) {
		*t = t_cap;
		hit = true;
	}
	return hit;
}*/
