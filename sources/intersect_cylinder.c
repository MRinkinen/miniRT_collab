/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:01:28 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 19:09:59 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

float	calculate_discriminant(t_cylinder_params *params)
{
	params->a = dot(params->direction, params->direction) - \
	pow(dot(params->direction, params->axis), 2);
	params->b = dot(params->direction, params->oc) - \
	dot(params->direction, params->axis) * dot(params->oc, params->axis);
	params->c = dot(params->oc, params->oc) - \
	pow(dot(params->oc, params->axis), 2) - params->radius * params->radius;
	return ((params->b * params->b) - (params->a * params->c));
}

bool	intersect_cylinder_body(const t_ray *ray, const t_cylinder \
*cylinder, float *t)
{
	t_cylinder_params	params;
	float				discriminant;
	t_t_values			t_vals;
	bool				hit;

	set_cylinder_params(ray, cylinder, &params);
	discriminant = calculate_discriminant(&params);
	hit = false;
	if (discriminant >= 0)
	{
		t_vals.t0 = (params.b - sqrt(discriminant)) / params.a;
		t_vals.t1 = (params.b + sqrt(discriminant)) / params.a;
		hit = check_cylinder_height(ray, cylinder, t, &t_vals);
	}
	return (hit);
}

bool	check_cap_intersection(const t_cap_params *params, t_tuple cap_center)
{
	t_tuple	cap_point;
	float	t_cap;
	float	cap_dist;

	t_cap = (dot(cap_center, params->cylinder->orientation) - \
			dot(params->ray->origin, params->cylinder->orientation)) / \
		dot(params->ray->direction, params->cylinder->orientation);
	if (t_cap < 0)
		return (params->hit);
	cap_point = tuple_add(params->ray->origin, \
	tuple_multiply(params->ray->direction, t_cap));
	cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), \
	tuple_subtract(cap_point, cap_center)));
	if (cap_dist <= params->cylinder->radius && \
	(!params->hit || t_cap < *(params->t)))
	{
		*(params->t) = t_cap;
		return (true);
	}
	return (params->hit);
}

bool	intersect_cylinder_caps(const t_ray *ray, const t_cylinder \
*cylinder, float *t, bool hit)
{
	t_tuple			cap_center_top;
	t_tuple			cap_center_bottom;
	t_cap_params	params;

	params.ray = ray;
	params.cylinder = cylinder;
	params.t = t;
	params.hit = hit;
	cap_center_top = tuple_add(cylinder->center, \
	tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	cap_center_bottom = tuple_subtract(cylinder->center, \
	tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	params.hit = check_cap_intersection(&params, cap_center_top);
	params.hit = check_cap_intersection(&params, cap_center_bottom);
	return (params.hit);
}

bool	intersect_cylinder(const t_ray *ray, \
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
