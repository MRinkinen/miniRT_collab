/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_ambient_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 20:27:57 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:35:06 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

int	setup_ambient(char **split, t_map *map)
{
	char	**rgb;

	map->ambient = malloc(sizeof(t_ambient));
	if (map->ambient == NULL)
		return (0);
	rgb = ft_split(split[2], ',');
	if (rgb == NULL)
		return (0);
	map->ambient->ratio = ft_atof(split[1]);
	map->ambient->r = ft_atoi(rgb[0]);
	map->ambient->g = ft_atoi(rgb[1]);
	map->ambient->b = ft_atoi(rgb[2]);
	free_split(rgb);
	return (1);
}
