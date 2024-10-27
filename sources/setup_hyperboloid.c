/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_hyperboloid.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:11:27 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/27 18:32:49 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static t_hyperboloids	*setup_hyperboloid_helper(t_map *map)
{
	t_hyperboloids	*temp;

	if (!map->hyperboloids)
	{
		map->hyperboloids = malloc(sizeof(t_hyperboloids));
		if (!map->hyperboloids)
			return (NULL);
		map->hyperboloids->next = NULL;
	}
	else
	{
		temp = map->hyperboloids;
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_hyperboloids));
		if (temp->next == NULL)
			return (NULL);
		temp->next->next = NULL;
		return (temp->next);
	}
	return (map->hyperboloids);
}

void	assign_hyperboloid_rgb(t_hyperboloids *hyperboloid, char **rgb)
{
	hyperboloid->r = ft_atoi(rgb[0]);
	hyperboloid->g = ft_atoi(rgb[1]);
	hyperboloid->b = ft_atoi(rgb[2]);
}

void	assign_hyperboloid_coordinates(t_hyperboloids *hyperboloid, \
char **xyz, char **nxyz, char **abc)
{
	hyperboloid->x = ft_atof(xyz[0]);
	hyperboloid->y = ft_atof(xyz[1]);
	hyperboloid->z = ft_atof(xyz[2]);
	hyperboloid->nx = ft_atof(nxyz[0]);
	hyperboloid->ny = ft_atof(nxyz[1]);
	hyperboloid->nz = ft_atof(nxyz[2]);
	hyperboloid->aa = ft_atof(abc[0]);
	hyperboloid->bb = ft_atof(abc[1]);
	hyperboloid->cc = ft_atof(abc[2]);
}

int	setup_hyperboloid(char **split, t_map *map)
{
	t_hyperboloids	*new_hyperboloid;
	char			**xyz;
	char			**nxyz;
	char			**abc;
	char			**rgb;

	xyz = ft_split(split[1], ',');
	nxyz = ft_split(split[2], ',');
	abc = ft_split(split[3], ',');
	rgb = ft_split(split[5], ',');
	new_hyperboloid = setup_hyperboloid_helper(map);
	if (new_hyperboloid == NULL)
		return (0);
	assign_hyperboloid_coordinates(new_hyperboloid, xyz, nxyz, abc);
	assign_hyperboloid_rgb(new_hyperboloid, rgb);
	new_hyperboloid->height = ft_atof(split[4]);
	new_hyperboloid->next = NULL;
	free_split(xyz);
	free_split(nxyz);
	free_split(rgb);
	return (1);
}
