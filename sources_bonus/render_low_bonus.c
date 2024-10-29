/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_low_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:20:55 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/29 10:10:23 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"

t_color	shade_pixel_low(t_var *var, t_object *closest_object)
{
	t_color	object_color;

	if (closest_object->type == SPHERE)
		object_color = closest_object->data.sphere.color;
	else if (closest_object->type == CYLINDER)
		object_color = closest_object->data.cylinder.color;
	else if (closest_object->type == PLANE)
		object_color = closest_object->data.plane.color;
	else
		object_color = (t_color){0, 0, 0};
	var->temp_color = object_color;
	return (object_color);
}

void	process_pixel_low(t_var *var, int x, int y)
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
		pixel_color = shade_pixel_low(var, closest_object);
	write_color(pixel_color, var, x, y);
}

void	printimage_low(void *param, int resolution_scale)
{
	t_var	*var;
	int		y;
	int		x;

	y = -1;
	var = param;
	while (++y < HEIGHT / resolution_scale)
	{
		x = -1;
		while (++x < WIDTH / resolution_scale)
		{
			process_pixel_low(var, x * resolution_scale, y * resolution_scale);
		}
	}
}
