/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:33:48 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/22 12:29:38 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static t_cylinders	*setup_cylinder_helper(t_map *map)
{
	t_cylinders	*temp;

	if (!map->cylinders)
	{
		map->cylinders = malloc(sizeof(t_cylinders));
		if (!map->cylinders)
			return (NULL);
		map->cylinders->next = NULL;
	}
	else
	{
		temp = map->cylinders;
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_cylinders));
		if (temp->next == NULL)
			return (NULL);
		temp->next->next = NULL;
		return (temp->next);
	}
	return (map->cylinders);
}

void	assign_cylinder_rgb(t_cylinders *cylinder, char **rgb)
{
	cylinder->r = ft_atoi(rgb[0]);
	cylinder->g = ft_atoi(rgb[1]);
	cylinder->b = ft_atoi(rgb[2]);
}

void	assign_cylinder_coordinates(t_cylinders *cylinder, \
char **xyz, char **nxyz)
{
	cylinder->x = -ft_atof(xyz[0]);
	cylinder->y = -ft_atof(xyz[1]);
	cylinder->z = ft_atof(xyz[2]);
	cylinder->nx = ft_atof(nxyz[0]);
	cylinder->ny = ft_atof(nxyz[1]);
	cylinder->nz = ft_atof(nxyz[2]);
}

int	setup_cylinder(char **split, t_map *map)
{
	t_cylinders	*new_cylinder;
	char		**xyz;
	char		**nxyz;
	char		**rgb;

	xyz = ft_split(split[1], ',');
	nxyz = ft_split(split[2], ',');
	rgb = ft_split(split[5], ',');
	new_cylinder = setup_cylinder_helper(map);
	if (new_cylinder == NULL)
		return (0);
	assign_cylinder_coordinates(new_cylinder, xyz, nxyz);
	assign_cylinder_rgb(new_cylinder, rgb);
	new_cylinder->diameter = ft_atof(split[3]);
	new_cylinder->height = ft_atof(split[4]);
	new_cylinder->next = NULL;
	free_split(xyz);
	free_split(nxyz);
	free_split(rgb);
	return (1);
}
