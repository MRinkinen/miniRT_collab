/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:33:48 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/16 12:12:59 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static t_cylinders	*setup_cylinder_helper(t_map *map)
{
	t_cylinders	*temp;

	if (!map->cylinders)
	{
		printf("inside setup_cylinder_helper 1\n");
		map->cylinders = malloc(sizeof(t_cylinders));
		if (!map->cylinders)
			return (NULL);
		map->cylinders->next = NULL;
	}
	else
	{
		printf("inside setup_cylinder_helper 2\n");
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

int	setup_cylinder(char **split, t_map *map)
{
	char	    	**xyz;
	char	    	**nxyz;
    char        	**rgb;
    t_cylinders  	*new_cylinder;

    printf("inside setup_cylinder\n");
	xyz = ft_split(split[1], ',');
	nxyz = ft_split(split[2], ',');
    rgb = ft_split(split[5], ',');
	new_cylinder = setup_cylinder_helper(map);
	if (new_cylinder == NULL)
		return (0);
    printf("inside setup_cylinder 1\n");
	new_cylinder->x = -ft_atof(xyz[0]);
	new_cylinder->y = -ft_atof(xyz[1]);
	new_cylinder->z = ft_atof(xyz[2]);
	new_cylinder->nx = ft_atof(nxyz[0]);
	new_cylinder->ny = ft_atof(nxyz[1]);
	new_cylinder->nz = ft_atof(nxyz[2]);
	new_cylinder->r = ft_atoi(rgb[0]);
    new_cylinder->g = ft_atoi(rgb[1]);
    new_cylinder->b = ft_atoi(rgb[2]);
    new_cylinder->diameter = ft_atof(split[3]);
    new_cylinder->height = ft_atof(split[4]);
	new_cylinder->next = NULL;
	free_split(xyz);
	free_split(nxyz);
    free_split(rgb);
    printf("end of setup_cylinder\n");
	return (1);
}
