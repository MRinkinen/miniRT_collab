/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_sphere_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:59:22 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:36:57 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

int	validate_sphere(char *line, t_element_count *element_count, t_map *map)
{
	char	**split;

	replace_tabs_with_spaces(line);
	split = ft_split(line, ' ');
	if (!split[1] || !split[2] || !split[3] || split[4])
		return (free_split(split));
	if (split[0][2] != '\0')
		return (free_split(split));
	if (xyz_check(split[1]) == 0)
		return (free_split(split));
	if (decimal_check(split[2], 0, 100) == 0)
		return (free_split(split));
	if (rgb_check(split[3], 0, 255) == 0)
		return (free_split(split));
	if (setup_spheres(split, map) == 0)
		return (free_split(split));
	free_split(split);
	element_count->sphere++;
	return (1);
}
