/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_camera_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:05:48 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:35:13 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

static t_camera	*setup_camera_helper(t_map *map)
{
	t_camera	*temp;

	if (!map->camera)
	{
		map->camera = malloc(sizeof(t_camera));
		if (!map->camera)
			return (NULL);
		map->camera->next = NULL;
	}
	else
	{
		temp = map->camera;
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_camera));
		if (temp->next == NULL)
			return (NULL);
		temp->next->next = NULL;
		return (temp->next);
	}
	return (map->camera);
}

int	setup_camera(char **split, t_map *map)
{
	char		**xyz;
	char		**nxyz;
	t_camera	*new_camera;

	xyz = ft_split(split[1], ',');
	nxyz = ft_split(split[2], ',');
	new_camera = setup_camera_helper(map);
	if (map->camera == NULL)
		return (0);
	new_camera->x = ft_atof(xyz[0]);
	new_camera->y = ft_atof(xyz[1]);
	new_camera->z = ft_atof(xyz[2]);
	new_camera->nx = ft_atof(nxyz[0]);
	new_camera->ny = ft_atof(nxyz[1]);
	new_camera->nz = ft_atof(nxyz[2]);
	new_camera->fov = ft_atof(split[3]);
	new_camera->next = NULL;
	free_split(xyz);
	free_split(nxyz);
	return (1);
}
