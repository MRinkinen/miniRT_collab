/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_seven.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:32:05 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 18:59:19 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// Function to create a point (w = 1.0)
t_tuple	point(double x, double y, double z)
{
	return (tuple(x, y, z, 1.0));
}

// Function to create a vector (w = 0.0)
t_tuple	vector(double x, double y, double z)
{
	return (tuple(x, y, z, 0.0));
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