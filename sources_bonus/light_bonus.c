/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 02:11:54 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:31:44 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"
#include "../includes_bonus/parsing_bonus.h"

t_light	light_create(t_tuple position, t_color intensity, float brightness)
{
	t_light	light;

	light.position = position;
	light.color = multiply_color_scalar(intensity, brightness);
	light.brightness = brightness;
	return (light);
}

bool	is_in_shadow(const t_tuple *point, t_light *light, \
t_object *objects, int num_objects)
{
	t_tuple	light_dir;
	t_tuple	point_offset;
	float	light_distance;
	float	threshold;
	int		i;

	i = 0;
	light_dir = normalize(tuple_subtract(light->position, *point));
	point_offset = tuple_add(*point, tuple_multiply(light_dir, 0.01f));
	light_distance = magnitude(tuple_subtract(light->position, point_offset));
	while (i < num_objects)
	{
		if (intersect_object(&(t_ray){point_offset, light_dir, \
		light->position}, &objects[i], &threshold) && \
		threshold > 0.001f && threshold < light_distance)
			return (true);
		i++;
	}
	return (false);
}

t_color	calculate_phong_lighting(t_var *var, const t_tuple *point, \
const t_tuple *normal, const t_tuple *view_dir)
{
	t_tuple	light_dir;
	float	diff_amount;
	float	spec;
	t_color	final_color;
	t_tuple	reflect_dir;

	light_dir = normalize(tuple_subtract(var->pointlights[0].position, *point));
	diff_amount = fmax(dot(*normal, light_dir), 0.0f);
	final_color = multiply_colors(var->temp_color, var->ambientl);
	if (!is_in_shadow(point, &var->pointlights[0], \
	var->objects, var->num_objects))
	{
		final_color = color_add(final_color, multiply_colors(var->temp_color, \
		multiply_color_scalar(var->pointlights[0].color, diff_amount)));
		reflect_dir = tuple_subtract(tuple_multiply(*normal, 2 * \
		dot(*normal, light_dir)), light_dir);
		spec = pow(fmax(dot(*view_dir, reflect_dir), 0.0f), 32);
		final_color = color_add(final_color, \
		multiply_color_scalar(var->pointlights[0].color, spec));
	}
	return (final_color);
}
