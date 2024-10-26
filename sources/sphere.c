/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 03:14:39 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 03:14:41 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

void	create_spheres(t_var *var, t_map *map, int *obj_index)
{
	t_spheres	*current_sphere;
	t_tuple		center;
	t_color		color;
	float		radius;

	current_sphere = map->spheres;
	while (current_sphere != NULL)
	{
		center = point(current_sphere->x, current_sphere->y, current_sphere->z);
		radius = current_sphere->diameter / 2.0f;
		color = t_color_create(current_sphere->r, current_sphere->g, \
		current_sphere->b);
		var->objects[*obj_index].type = SPHERE;
		var->objects[*obj_index].data.sphere.center = center;
		var->objects[*obj_index].data.sphere.radius = radius;
		var->objects[*obj_index].data.sphere.color = color;
		current_sphere = current_sphere->next;
		(*obj_index)++;
	}
}

t_tuple	calculate_sphere_normal(const t_sphere *sphere, const t_tuple *point)
{
	return (normalize(tuple_subtract(*point, sphere->center)));
}

bool	intersect_sphere(const t_ray *ray, const t_sphere *sphere, float *t)
{
	t_tuple	oc;
	float	a;
	float	b;
	float	c;
	float	discriminant;

	oc = tuple_subtract(ray->origin, sphere->center);
	a = dot(ray->direction, ray->direction);
	b = 2.0f * dot(oc, ray->direction);
	c = dot(oc, oc) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (false);
	else
	{
		*t = (-b - sqrt(discriminant)) / (2.0f * a);
		return (true);
	}
}

/*
t_sphere sphere_create(t_tuple center, float radius, t_color col)
{
	//printf("Creating sphere at (%f, %f, %f) with radius %f\n", center.x, center.y, center.z, radius);
	t_sphere sphere;

	//hittable_init(&sphere.base, sphere_hit);
	sphere.color = col;
	sphere.center = center;
	sphere.radius = fmax(0, radius);

	 // Initialize transformation matrices
	sphere.translation_matrix = translation(center.x, center.y, center.z);
	sphere.rotation_matrix = identity_matrix(); // No rotation initially
	sphere.scaling_matrix = scaling(radius, radius, radius);
	//printf("CHECK THIIIS!!!!\n");
	//printf("Sphere Center Z: %f\n", center.z);
	//printf("Translation Matrix Z-component: %f\n", sphere.translation_matrix->data[2][3]);

	// Combine transformations into one matrix
	sphere.transform = t_matrix_multiply(t_matrix_multiply(sphere.translation_matrix, sphere.rotation_matrix), sphere.scaling_matrix);

	// Calculate the inverse transform for ray-sphere intersection
	sphere.inverse_transform = inverse(sphere.transform);
	return (sphere);
}

t_tuple calculate_sphere_normal(const t_sphere *sphere, const t_tuple *point)
{
	return normalize(tuple_subtract(*point, sphere->center));
}

bool intersect_sphere(const t_ray *ray, const t_sphere *sphere, float *t)
{
	t_tuple oc = tuple_subtract(ray->origin, sphere->center);
	float a = dot(ray->direction, ray->direction);
	float b = 2.0f * dot(oc, ray->direction);
	float c = dot(oc, oc) - sphere->radius * sphere->radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return false;
	} else {
		*t = (-b - sqrt(discriminant)) / (2.0f * a);
		return true;
	}
}*/

