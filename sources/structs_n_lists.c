/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs_n_lists.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 11:10:02 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 17:16:29 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

void	free_cylinder(t_cylinder *cylinder)
{
	printf("in free cylinder\n");
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

int	setup_data(t_element_count *element_count, t_map *map)
{
	map->element_count = element_count;
	return (1);
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

void free_cameras(t_camera *camera)
{
	t_camera	*next_camera;

	while (camera)
	{
		next_camera = camera->next;
		free(camera);
		camera = next_camera;
	}
}

void free_lights(t_lights *lights)
{
	t_lights	*next_light;

	while (lights)
	{
		next_light = lights->next;
		free(lights);
		lights = next_light;
	}
}

void free_spheres(t_spheres *sphere)
{
	t_spheres	*next_sphere;

	while (sphere)
	{
		next_sphere = sphere->next;
		free(sphere);
		sphere = next_sphere;
	}
}

void free_planes(t_planes *plane)
{
	t_planes	*next_plane;

	while (plane)
	{
		next_plane = plane->next;
		free(plane);
		plane = next_plane;
	}
}

void free_cylinders(t_cylinders *cylinder)
{
	t_cylinders	*next_cylinder;

	while (cylinder)
	{
		next_cylinder = cylinder->next;
		free(cylinder);
		cylinder = next_cylinder;
	}
}

int terminate_map_data(t_map *map, t_var *var, char *error)
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
