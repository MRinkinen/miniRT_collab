/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_light_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:26:31 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:35:25 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

static t_lights	*setup_light_helper(t_map *map)
{
	t_lights	*temp;

	if (!map->lights)
	{
		map->lights = malloc(sizeof(t_lights));
		if (!map->lights)
			return (NULL);
		map->lights->next = NULL;
	}
	else
	{
		temp = map->lights;
		while (temp->next)
			temp = temp->next;
		temp->next = malloc(sizeof(t_lights));
		if (temp->next == NULL)
			return (NULL);
		temp->next->next = NULL;
		return (temp->next);
	}
	return (map->lights);
}

int	setup_light(char **split, t_map *map)
{
	char		**rgb;
	char		**xyz;
	t_lights	*new_light;

	xyz = ft_split(split[1], ',');
	rgb = ft_split(split[3], ',');
	new_light = setup_light_helper(map);
	if (map->lights == NULL)
		return (0);
	new_light->x = ft_atof(xyz[0]);
	new_light->y = ft_atof(xyz[1]);
	new_light->z = ft_atof(xyz[2]);
	new_light->ratio = ft_atof(split[2]);
	new_light->r = ft_atoi(rgb[0]);
	new_light->g = ft_atoi(rgb[1]);
	new_light->b = ft_atoi(rgb[2]);
	new_light->next = NULL;
	free_split(xyz);
	free_split(rgb);
	return (1);
}
