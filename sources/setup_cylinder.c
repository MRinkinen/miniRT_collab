/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:33:48 by tvalimak          #+#    #+#             */
/*   Updated: 2024/08/20 13:49:58 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static t_cylinder	*setup_cylinder_helper(t_map *map)
{
	t_cylinder	*temp;

	if (!map->cylinder)
	{
		printf("inside setup_cylinder_helper 1\n");
		map->cylinder = malloc(sizeof(t_cylinder));
		if (!map->cylinder)
			return (NULL);
		map->cylinder->next = NULL;
	}
	else
	{
		printf("inside setup_cylinder_helper 2\n");
		temp = map->cylinder;
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_cylinder));
		if (temp->next == NULL)
			return (NULL);
		temp->next->next = NULL;
		return temp->next;
	}
	return map->cylinder;
}

int	setup_cylinder(char **split, t_map *map)
{
	char	    **xyz;
	char	    **nxyz;
    char        **rgb;
    t_cylinder  *new_cylinder;

    printf("inside setup_cylinder\n");
	xyz = ft_split(split[1], ',');
	nxyz = ft_split(split[2], ',');
    rgb = ft_split(split[5], ',');
	new_cylinder = setup_cylinder_helper(map);
	if (new_cylinder == NULL)
		return (0);
    printf("inside setup_cylinder 1\n");
	new_cylinder->x = ft_atof(xyz[0]);
	new_cylinder->y = ft_atof(xyz[1]);
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
