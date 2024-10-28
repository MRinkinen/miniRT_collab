/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 03:13:17 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 21:24:46 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

int	init_objects(t_var *var, t_map *map)
{
	int	obj_index;

	obj_index = 0;
	var->num_objects = map->element_count->sphere + \
	map->element_count->cylinder + map->element_count->plane;
	var->objects = malloc(var->num_objects * sizeof(t_object));
	if (var->objects == NULL)
		return (EXIT_FAILURE);
	create_spheres(var, map, &obj_index);
	create_cylinders(var, map, &obj_index);
	create_planes(var, map, &obj_index);
	return (EXIT_SUCCESS);
}

int	init_light(t_var *var, t_map *map)
{
	int			i;
	float		brightness;
	t_lights	*current_light;
	t_tuple		position;
	t_color		intensity;

	i = 0;
	current_light = map->lights;
	var->num_lights = map->element_count->light;
	var->pointlights = malloc(var->num_lights * sizeof(t_light));
	if (!var->pointlights)
		return (EXIT_FAILURE);
	while (current_light != NULL)
	{
		position = point(current_light->x, current_light->y, current_light->z);
		intensity = t_color_create(current_light->r, \
		current_light->b, current_light->g);
		brightness = current_light->ratio;
		var->pointlights[i] = light_create(position, intensity, brightness);
		i++;
		current_light = current_light->next;
	}
	return (EXIT_SUCCESS);
}

int	init_ambient_color(t_var *var, t_map *map)
{
	t_color	ambient;

	ambient = t_color_create(map->ambient->r, map->ambient->g, map->ambient->b);
	var->ambientl = multiply_color_scalar(ambient, map->ambient->ratio);
	return (EXIT_SUCCESS);
}
