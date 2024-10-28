/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_six.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:30:50 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 19:08:40 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_matrix	*tuple_to_matrix(t_tuple *t)
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

t_tuple	apply_transformation(t_matrix *transformation, t_tuple *point)
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
