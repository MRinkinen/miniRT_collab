/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:29:43 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/27 16:13:41 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

static void	free_cylinder(t_cylinder *cylinder)
{
	if (cylinder)
	{
		free(cylinder->inverse_transform);
		free(cylinder->transform);
		free(cylinder->scaling_matrix);
		free(cylinder->translation_matrix);
		free(cylinder->rotation_matrix);
	}
}

void	terminate_var_data(t_var *var, char *error)
{
	int	obj_index;

	obj_index = 0;
	if (error)
		printf("%s\n", error);
	while (var->objects && obj_index < var->num_objects)
	{
		if (var->objects[obj_index].type == CYLINDER)
		{
			free_cylinder(&(var->objects[obj_index].data.cylinder));
		}
		obj_index++;
	}
	free(var->pointlights);
	free(var->objects);
	if (var->screenimage)
		mlx_delete_image(var->mlx, var->screenimage);
	if (var->mlx)
		mlx_terminate(var->mlx);
}

int	free_split(char **split)
{
	int	i;

	if (split == NULL)
		return (0);
	i = 0;
	while (split[i] != NULL)
	{
		free(split[i]);
		split[i] = NULL;
		i++;
	}
	free(split);
	split = NULL;
	return (0);
}

int	free_scene(t_var *var)
{
	if (var->objects != NULL)
	{
		free(var->objects);
		var->objects = NULL;
	}
	return (1);
}

int	terminate_map_data(t_map *map, char *error)
{
	if (error)
		printf("%s\n", error);
	if (map->ambient)
		free(map->ambient);
	free_cameras(map->camera);
	free_lights(map->lights);
	free_spheres(map->spheres);
	free_planes(map->planes);
	free_cylinders(map->cylinders);
	free(map);
	return (1);
}
