/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_two_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:37:24 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:30:21 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"
#include "../includes_bonus/parsing_bonus.h"

bool	is_within_height(float dist, float half_height)
{
	return (dist >= -half_height && dist <= half_height);
}

float	calculate_distance(const t_tuple *point, const t_cylinder *cylinder)
{
	return (dot(*point, cylinder->orientation) - \
	dot(cylinder->center, cylinder->orientation));
}

bool	check_cylinder_height(const t_ray *ray, const t_cylinder *cylinder, \
float *t, const t_t_values *t_vals)
{
	t_tuple	point;
	float	dist;
	float	half_height;

	half_height = cylinder->height / 2.0;
	point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_vals->t0));
	dist = calculate_distance(&point, cylinder);
	if (is_within_height(dist, half_height))
	{
		*t = t_vals->t0;
		return (true);
	}
	point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_vals->t1));
	dist = calculate_distance(&point, cylinder);
	if (is_within_height(dist, half_height))
	{
		*t = t_vals->t1;
		return (true);
	}
	return (false);
}
