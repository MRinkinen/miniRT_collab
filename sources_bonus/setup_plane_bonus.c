/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_plane_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:24:59 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:35:32 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

static t_planes	*setup_plane_helper(t_map *map)
{
	t_planes	*temp;

	if (!map->planes)
	{
		map->planes = malloc(sizeof(t_planes));
		if (!map->planes)
			return (NULL);
		map->planes->next = NULL;
	}
	else
	{
		temp = map->planes;
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_planes));
		if (temp->next == NULL)
			return (NULL);
		temp->next->next = NULL;
		return (temp->next);
	}
	return (map->planes);
}

int	setup_plane(char **split, t_map *map)
{
	char		**xyz;
	char		**nxyz;
	char		**rgb;
	t_planes	*new_plane;

	xyz = ft_split(split[1], ',');
	nxyz = ft_split(split[2], ',');
	rgb = ft_split(split[3], ',');
	new_plane = setup_plane_helper(map);
	if (map->planes == NULL)
		return (0);
	new_plane->x = ft_atof(xyz[0]);
	new_plane->y = ft_atof(xyz[1]);
	new_plane->z = ft_atof(xyz[2]);
	new_plane->nx = ft_atof(nxyz[0]);
	new_plane->ny = ft_atof(nxyz[1]);
	new_plane->nz = ft_atof(nxyz[2]);
	new_plane->r = ft_atoi(rgb[0]);
	new_plane->g = ft_atoi(rgb[1]);
	new_plane->b = ft_atoi(rgb[2]);
	new_plane->next = NULL;
	free_split(xyz);
	free_split(nxyz);
	free_split(rgb);
	return (1);
}
