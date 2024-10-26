/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderhelper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:07:53 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 17:07:55 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

bool	intersect_object(t_ray *ray, t_object *object, float *t)
{
	if (object->type == SPHERE)
	{
		return (intersect_sphere(ray, &object->data.sphere, t));
	}
	else if (object->type == CYLINDER)
	{
		return (intersect_cylinder(ray, &object->data.cylinder, t));
	}
	else if (object->type == PLANE)
	{
		return (intersect_plane(ray, &object->data.plane, t));
	}
	else
	{
		return (false);
	}
}

bool	find_closest_intersection(t_var *var, t_ray *ray, \
t_object **closest_object, float *closest_t)
{
	bool	hit;
	float	t;
	int		i;

	i = 0;
	hit = false;
	*closest_t = FLT_MAX;
	while (i < var->num_objects)
	{
		if (intersect_object(ray, &var->objects[i], &t) \
		&& t < *closest_t && t > 0.001f)
		{
			*closest_t = t;
			*closest_object = &var->objects[i];
			hit = true;
		}
		i++;
	}
	return (hit);
}

t_tuple	calculate_normal(t_object *object, t_tuple *point)
{
	if (object->type == SPHERE)
		return (calculate_sphere_normal(&object->data.sphere, point));
	else if (object->type == CYLINDER)
		return (calculate_cylinder_normal(&object->data.cylinder, point));
	else if (object->type == PLANE)
		return (object->data.plane.normal);
	else
		return (vector(0, 0, 0));
}
