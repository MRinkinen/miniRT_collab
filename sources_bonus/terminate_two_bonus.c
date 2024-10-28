/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_two_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:31:18 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:36:07 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"
#include "../includes_bonus/parsing_bonus.h"

void	free_cameras(t_camera *camera)
{
	t_camera	*next_camera;

	while (camera)
	{
		next_camera = camera->next;
		free(camera);
		camera = next_camera;
	}
}

void	free_lights(t_lights *lights)
{
	t_lights	*next_light;

	while (lights)
	{
		next_light = lights->next;
		free(lights);
		lights = next_light;
	}
}

void	free_spheres(t_spheres *sphere)
{
	t_spheres	*next_sphere;

	while (sphere)
	{
		next_sphere = sphere->next;
		free(sphere);
		sphere = next_sphere;
	}
}

void	free_planes(t_planes *plane)
{
	t_planes	*next_plane;

	while (plane)
	{
		next_plane = plane->next;
		free(plane);
		plane = next_plane;
	}
}

void	free_cylinders(t_cylinders *cylinder)
{
	t_cylinders	*next_cylinder;

	while (cylinder)
	{
		next_cylinder = cylinder->next;
		free(cylinder);
		cylinder = next_cylinder;
	}
}
