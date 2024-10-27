/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_hyperboloid.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:49:53 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/27 19:52:46 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

float	calculate_hyperboloid_discriminant(t_ray *ray, \
t_hyperboloid *hyperboloid, t_hyperboloid_params *params)
{
	params->a = (ray->direction.x * ray->direction.x) / \
	(hyperboloid->a * hyperboloid->a)
		+ (ray->direction.y * ray->direction.y) / \
			(hyperboloid->b * hyperboloid->b)
		- (ray->direction.z * ray->direction.z) / \
		(hyperboloid->c * hyperboloid->c);
	params->b = 2.0 * ((ray->origin.x * ray->direction.x) / \
	(hyperboloid->a * hyperboloid->a)
			+ (ray->origin.y * ray->direction.y) / \
			(hyperboloid->b * hyperboloid->b)
			- (ray->origin.z * ray->direction.z) / \
			(hyperboloid->c * hyperboloid->c));
	params->c = (ray->origin.x * ray->origin.x) / \
	(hyperboloid->a * hyperboloid->a)
		+ (ray->origin.y * ray->origin.y) / \
			(hyperboloid->b * hyperboloid->b)
		- (ray->origin.z * ray->origin.z) / \
			(hyperboloid->c * hyperboloid->c) - 1;
	return ((params->b * params->b) - (4 * params->a * params->c));
}

t_tuple	calculate_hyperboloid_normal(t_hyperboloid *hyperboloid, t_tuple *point)
{
	t_tuple	normal;

	hyperboloid->orientation = normalize(hyperboloid->orientation);
	normal.x = (2 * point->x) / (hyperboloid->a * hyperboloid->a);
	normal.y = (2 * point->y) / (hyperboloid->b * hyperboloid->b);
	normal.z = -(2 * point->z) / (hyperboloid->c * hyperboloid->c);
	normal.w = 0;
	normal = normalize(normal);
	return (normal);
}

bool	hyper_is_within_height(float dist, float half_height)
{
	return (dist >= -half_height && dist <= half_height);
}

float	hyper_calculate_distance(t_tuple *point, t_hyperboloid *hyperboloid)
{
	return (dot(*point, hyperboloid->orientation) - \
	dot(hyperboloid->center, hyperboloid->orientation));
}

bool	check_hyperboloid_height(t_ray *ray, t_hyperboloid *hyperboloid, \
float *t, t_t_values *t_vals)
{
	t_tuple	point;
	float	dist;
	float	half_height;

	half_height = hyperboloid->height / 2.0;
	point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_vals->t0));
	dist = hyper_calculate_distance(&point, hyperboloid);
	if (hyper_is_within_height(dist, half_height))
	{
		*t = t_vals->t0;
		return (true);
	}
	point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_vals->t1));
	dist = hyper_calculate_distance(&point, hyperboloid);
	if (hyper_is_within_height(dist, half_height))
	{
		*t = t_vals->t1;
		return (true);
	}
	return (false);
}

void	set_hyperboloid_params(t_ray *ray, t_hyperboloid *hyperboloid, \
t_hyperboloid_params *params)
{
	params->oc = tuple_subtract(hyperboloid->center, ray->origin);
	params->direction = ray->direction;
	params->axis = hyperboloid->orientation;
	params->a = hyperboloid->a;
	params->b = hyperboloid->b;
	params->c = hyperboloid->c;
}

bool	intersect_hyperboloid_body(t_ray *ray, t_hyperboloid \
*hyperboloid, float *t)
{
	t_hyperboloid_params	params;
	float					discriminant;
	t_t_values				t_vals;
	bool					hit;

	set_hyperboloid_params(ray, hyperboloid, &params);
	discriminant = calculate_hyperboloid_discriminant(ray, \
	hyperboloid, &params);
	hit = false;
	if (discriminant >= 0)
	{
		t_vals.t0 = (params.b - sqrt(discriminant)) / params.a;
		t_vals.t1 = (params.b + sqrt(discriminant)) / params.a;
		hit = check_hyperboloid_height(ray, hyperboloid, t, &t_vals);
	}
	return (hit);
}
/*
bool	intersect_hyperboloid(t_ray *ray, t_hyperboloid *hyperboloid, float *t)
{
	t_ray	transformed_ray;
	bool	hit;

	// Print original ray data
	printf("Original Ray:\n");
	printf("  Origin:    (%f, %f, %f)\n", ray->origin.x, ray->origin.y, ray->origin.z);
	printf("  Direction: (%f, %f, %f)\n", ray->direction.x, ray->direction.y, ray->direction.z);

	// Print hyperboloid transformation data
	printf("Hyperboloid Transformations:\n");
	printf("  Inverse Transform Matrix:\n");
	for (int i = 0; i < 4; i++)
	{
		printf("    [%f, %f, %f, %f]\n", 
			hyperboloid->inverse_transform->data[i][0], 
			hyperboloid->inverse_transform->data[i][1], 
			hyperboloid->inverse_transform->data[i][2], 
			hyperboloid->inverse_transform->data[i][3]);
	}

	// Transform the ray into the hyperboloid's local space
	transformed_ray.origin = apply_transformation(hyperboloid->inverse_transform, &ray->origin);
	transformed_ray.direction = apply_transformation(hyperboloid->inverse_transform, &ray->direction);

	// Print transformed ray data
	printf("Transformed Ray:\n");
	printf("  Origin:    (%f, %f, %f)\n", transformed_ray.origin.x, transformed_ray.origin.y, transformed_ray.origin.z);
	printf("  Direction: (%f, %f, %f)\n", transformed_ray.direction.x, transformed_ray.direction.y, transformed_ray.direction.z);

	// Perform the intersection check using the transformed ray
	hit = intersect_hyperboloid_body(&transformed_ray, hyperboloid, t);

	// Print intersection result
	if (hit)
	{
		printf("Intersection detected at t = %f\n", *t);
	}
	else
	{
		printf("No intersection detected.\n");
	}

	return hit;
}*/

bool	intersect_hyperboloid(t_ray *ray, \
t_hyperboloid *hyperboloid, float *t)
{
	t_ray	transformed_ray;
	bool	hit;

	transformed_ray.origin = \
	apply_transformation(hyperboloid->inverse_transform, &ray->origin);
	transformed_ray.direction = \
	apply_transformation(hyperboloid->inverse_transform, &ray->direction);
	hit = intersect_hyperboloid_body(&transformed_ray, hyperboloid, t);
	return (hit);
}
