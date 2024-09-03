/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:05:48 by tvalimak          #+#    #+#             */
/*   Updated: 2024/09/03 20:55:56 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static t_camera	*setup_camera_helper(t_map *map)
{
	t_camera	*temp;

	if (!map->camera)
	{
		printf("inside setup_camera_helper 1\n");
		map->camera = malloc(sizeof(t_camera));
		if (!map->camera)
			return (NULL);
		map->camera->next = NULL;
	}
	else
	{
		printf("inside setup_camera_helper 2\n");
		temp = map->camera;
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_camera));
		if (temp->next == NULL)
			return (NULL);
		temp->next->next = NULL;
		return temp->next;
	}
	return map->camera;
}

/*identifier: C
∗ x,y,z coordinates of the view point: -50.0,0,20
∗ 3d normalized orientation vector. In range [-1,1] for each x,y,z axis:
0.0,0.0,1.0
∗ FOV : Horizontal field of view in degrees in range [0,180]: 70*/

int	setup_camera(char **split, t_map *map)
{
	char		**xyz;
	char		**nxyz;
	t_camera	*new_camera;

	printf("inside setup_camera 0\n");
	xyz = ft_split(split[1], ',');
	nxyz = ft_split(split[2], ',');
	printf("inside setup_camera 1\n");
	new_camera = setup_camera_helper(map);
	if (map->camera == NULL)
		return (0);
	printf("inside setup_camera 2\n");
	new_camera->x = ft_atof(xyz[0]);
	new_camera->y = ft_atof(xyz[1]);
	new_camera->z = ft_atof(xyz[2]);
	printf("inside setup_camera 3\n");
	new_camera->nx = ft_atof(nxyz[0]);
	new_camera->ny = ft_atof(nxyz[1]);
	new_camera->nz = ft_atof(nxyz[2]);
	printf("inside setup_camera 4\n");
	new_camera->fov = ft_atof(split[3]);
	new_camera->next = NULL;
	free_split(xyz);
	free_split(nxyz);
	printf("end of setup_camera\n");
	return (1);
}
