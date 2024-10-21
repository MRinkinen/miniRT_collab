/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:18:40 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/21 17:54:54 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

// Function to calculate the normal vector at a point on the cylinder's surface
t_tuple	calculate_cylinder_normal(const t_cylinder *cylinder, \
	const t_tuple *point)
{
	t_tuple	center_to_point;
	t_tuple	normal;

	center_to_point = tuple_subtract(*point, cylinder->center);
	normal = tuple(center_to_point.x, 0.0f, center_to_point.z, 0);
	return (normalize(normal));
}

t_tuple	tuple_scale(t_tuple t, double scalar)
{
	return (tuple_multiply(t, scalar));
}


bool	intersect_cylinder(const t_ray *ray, \
const t_cylinder *cylinder, float *t)
{
	t_tuple	oc;
	t_tuple	axis;
	t_tuple	direction;
	t_tuple point0;
	t_tuple point1;
	t_tuple cap_center;
	t_tuple cap_point;
	t_ray	transformed_ray;
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	t0;
	float	t1;
	float	temp;
	float	half_height;
	float	dist0;
	float	dist1;
	float	t_cap;
	float	cap_dist;
	bool	hit;

	transformed_ray.origin = apply_transformation(cylinder->inverse_transform, \
	&ray->origin);
	transformed_ray.direction = \
	apply_transformation(cylinder->inverse_transform, &ray->direction);
	oc = tuple_subtract(cylinder->center, ray->origin);
	direction = ray->direction;
	axis = cylinder->orientation;
	a = dot(direction, direction) - pow(dot(direction, axis), 2);
	b = (dot(direction, oc) - dot(direction, axis) * dot(oc, axis));
	c = dot(oc, oc) - pow(dot(oc, axis), \
	2) - cylinder->radius * cylinder->radius;
	discriminant = (b * b) - (a * c);
	if (discriminant >= 0)
	{
		t0 = (b - sqrt(discriminant)) / a;
		t1 = (b + sqrt(discriminant)) / a;
		if (t0 > t1)
		{
			temp = t0;
			t0 = t1;
			t1 = temp;
		}
		half_height = cylinder->height / 2.0;
		point0 = tuple_add(ray->origin, tuple_multiply(ray->direction, t0));
		point1 = tuple_add(ray->origin, tuple_multiply(ray->direction, t1));
		dist0 = dot(point0, axis) - dot(cylinder->center, axis);
		dist1 = dot(point1, axis) - dot(cylinder->center, axis);
		if (dist0 >= -half_height && dist0 <= half_height)
		{
			*t = t0;
			hit = true;
		}
		else if (dist1 >= -half_height && dist1 <= half_height)
		{
			*t = t1;
			hit = true;
		}
	}
	// Check for intersection with the caps
	// Top cap
	cap_center = tuple_add(cylinder->center, \
	tuple_multiply(axis, cylinder->height / 2.0));
	t_cap = (dot(cap_center, axis) - \
	dot(ray->origin, axis)) / dot(ray->direction, axis);
	cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
	cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), \
	tuple_subtract(cap_point, cap_center)));
	if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t))
	{
		*t = t_cap;
		hit = true;
	}
	// Bottom cap
	cap_center = tuple_subtract(cylinder->center, \
	tuple_multiply(axis, cylinder->height / 2.0));
	t_cap = (dot(cap_center, axis) - \
	dot(ray->origin, axis)) / dot(ray->direction, axis);
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
