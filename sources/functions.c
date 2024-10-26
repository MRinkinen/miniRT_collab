/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:18 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/26 17:39:31 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// Function to return the determinant of a 2x2 matrix
float	determinant_2x2(const t_matrix *m)
{
	return (m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0]);
}

// Function to return the determinant of a 3x3 matrix
float	determinant_3x3(const t_matrix *m)
{
	return (m->data[0][0] * (m->data[1][1] * m->data[2][2] - \
			m->data[1][2] * m->data[2][1]) -
			m->data[0][1] * (m->data[1][0] * m->data[2][2] - \
			m->data[1][2] * m->data[2][0]) +
			m->data[0][2] * (m->data[1][0] * m->data[2][1] - \
			m->data[1][1] * m->data[2][0]));
}

float	determinant_4x4(const t_matrix *m)
{
	int		col;
	float	det;

	det = 0.0;
	col = 0;
	while (col < m->cols)
	{
		det += m->data[0][col] * cofactor(m, 0, col);
		col++;
	}
	return (det);
}

// Function to return the determinant of a matrix
float	determinant(const t_matrix *m)
{
	if (m->rows == 2 && m->cols == 2)
		return (determinant_2x2(m));
	if (m->rows == 3 && m->cols == 3)
		return (determinant_3x3(m));
	if (m->rows == 4 && m->cols == 4)
		return (determinant_4x4(m));

	printf("Determinant calculation not implemented \
	for matrices larger than 4x4.\n");
	return (0.0f);
}

// Function to create an identity matrix
t_matrix	*identity_matrix()
{
	int			i;
	int			j;
	t_matrix	*identity;

	identity = (t_matrix *)malloc(sizeof(t_matrix));
	if (identity == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	identity->rows = 4;
	identity->cols = 4;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			if (i == j)
				identity->data[i][j] = 1.0;
			else
				identity->data[i][j] = 0.0;
		}
	}
	return (identity);
}

// Function to transpose a matrix
t_matrix*	transpose(t_matrix *m)
{
	int			i;
	int			j;
	t_matrix	*transposed;

	transposed = (t_matrix *)malloc(sizeof(t_matrix));
	if (transposed == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	transposed->rows = m->cols;
	transposed->cols = m->rows;
	i = 0;
	while (i < m->rows)
	{
		j = 0;
		while (j < m->cols)
		{
			transposed->data[j][i] = m->data[i][j];
			j++;
		}
		i++;
	}
	return (transposed);
}

void multiply_elements(t_matrix *result, t_matrix *a, t_matrix *b)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result->data[i][j] = 0;
			k = 0;
			while (k < 4)
			{
				result->data[i][j] += a->data[i][k] * b->data[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

t_matrix *t_matrix_multiply(t_matrix *a, t_matrix *b)
{
	t_matrix	*result;

	if (a->rows != 4 || a->cols != 4 || b->rows != 4 || b->cols != 4)
	{
		printf("Error: Both matrices must be 4x4.\n");
		return (NULL);
	}
	result = (t_matrix *)malloc(sizeof(t_matrix));
	if (result == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	result->rows = 4;
	result->cols = 4;
	multiply_elements(result, a, b);
	return (result);
}

// Function to create a 4x4 matrix from an array of values
t_matrix *create_4x4_matrix(float values[16])
{
	int				row;
	int				col;
	t_matrix		*matrix;

	matrix = (t_matrix *)malloc(sizeof(t_matrix));
	if (matrix == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	matrix->rows = 4;
	matrix->cols = 4;
	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			matrix->data[row][col] = values[row * 4 + col];
			col++;
		}
		row++;
	}
	return (matrix);
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
