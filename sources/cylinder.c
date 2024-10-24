/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:20:42 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/25 00:20:44 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

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
}
