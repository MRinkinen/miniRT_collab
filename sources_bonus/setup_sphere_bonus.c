/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_sphere_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:59:52 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:35:38 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

static t_spheres	*setup_spheres_helper(t_map *map)
{
	t_spheres	*temp;

	if (!map->spheres)
	{
		map->spheres = malloc(sizeof(t_spheres));
		if (!map->spheres)
			return (NULL);
		map->spheres->next = NULL;
	}
	else
	{
		temp = map->spheres;
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_spheres));
		if (temp->next == NULL)
			return (NULL);
		temp->next->next = NULL;
		return (temp->next);
	}
	return (map->spheres);
}

int	setup_spheres(char **split, t_map *map)
{
	char		**rgb;
	char		**xyz;
	t_spheres	*new_sphere;

	new_sphere = setup_spheres_helper(map);
	rgb = ft_split(split[3], ',');
	xyz = ft_split(split[1], ',');
	new_sphere->x = ft_atof(xyz[0]);
	new_sphere->y = ft_atof(xyz[1]);
	new_sphere->z = ft_atof(xyz[2]);
	new_sphere->diameter = ft_atof(split[2]);
	new_sphere->r = ft_atoi(rgb[0]);
	new_sphere->g = ft_atoi(rgb[1]);
	new_sphere->b = ft_atoi(rgb[2]);
	free_split(rgb);
	free_split(xyz);
	return (1);
}
