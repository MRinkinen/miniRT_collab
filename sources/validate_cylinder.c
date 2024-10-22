/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_cylinder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:14:50 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/22 13:15:59 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	validate_cylinder(char *line, t_element_count *element_count, \
	t_map *map)
{
	char	**split;

	replace_tabs_with_spaces(line);
	split = ft_split(line, ' ');
	if (!split[1] || !split[2] || !split[3] || \
	!split[4] || !split[5] || split[6])
		return (free_split(split));
	if (split[0][2] != '\0')
		return (free_split(split));
	if (xyz_check(split[1]) == 0)
		return (free_split(split));
	if (vectors_check(split[2]) == 0)
		return (free_split(split));
	if (decimal_check(split[3], 0, 100) == 0)
		return (free_split(split));
	if (decimal_check(split[4], 0, 100) == 0)
		return (free_split(split));
	if (rgb_check(split[5], 0, 256) == 0)
		return (free_split(split));
	if (setup_cylinder(split, map) == 0)
		return (free_split(split));
	free_split(split);
	element_count->cylinder++;
	return (1);
}
