/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:07:42 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/27 19:51:23 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color	shade_pixel(t_var *var, t_ray *r, \
t_object *closest_object, float closest_t)
{
	t_tuple	intersection_point;
	t_tuple	normal;
	t_color	object_color;
	t_tuple	view_dir;

	intersection_point = tuple_add(r->origin, \
	tuple_multiply(r->direction, closest_t));
	normal = calculate_normal(closest_object, &intersection_point);
	view_dir = normalize(tuple_subtract(var->cam.position, intersection_point));
	if (closest_object->type == SPHERE)
		object_color = closest_object->data.sphere.color;
	else if (closest_object->type == CYLINDER)
		object_color = closest_object->data.cylinder.color;
	else if (closest_object->type == HYPERBOLOID)
		object_color = closest_object->data.hyperboloid.color;
	else if (closest_object->type == PLANE)
		object_color = closest_object->data.plane.color;
	else
		object_color = (t_color){0, 0, 0};
	var->temp_color = object_color;
	return (calculate_phong_lighting(var, \
	&intersection_point, &normal, &view_dir));
}

void	process_pixel(t_var *var, int x, int y)
{
	t_ray		r;
	t_color		pixel_color;
	t_object	*closest_object;
	float		closest_t;

	closest_object = NULL;
	pixel_color = var->ambientl;
	r.px_center = tuple_add(var->cam.loc_00, \
	tuple_multiply(var->cam.delta_u, x));
	r.px_center = tuple_add(r.px_center, tuple_multiply(var->cam.delta_v, y));
	r.direction = normalize(tuple_subtract(r.px_center, var->cam.position));
	if (dot(r.direction, var->cam.forward) < 0)
		r.direction = tuple_multiply(r.direction, -1.0);
	r = ray(var->cam.position, r.direction);
	if (find_closest_intersection(var, &r, &closest_object, &closest_t))
		pixel_color = shade_pixel(var, &r, closest_object, closest_t);
	write_color(pixel_color, var, x, y);
}

void	printimage(void *param)
{
	t_var	*var;
	int		y;
	int		x;

	printf("object type in var:%d\n", var->objects->type);
	y = -1;
	var = param;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
		{
			process_pixel(var, x, y);
		}
	}
}
