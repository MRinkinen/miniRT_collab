/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_light_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:51:00 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:36:44 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

int	validate_light(char *line, t_element_count *element_count, t_map *map)
{
	char	**split;

	(void)element_count;
	replace_tabs_with_spaces(line);
	split = ft_split(line, ' ');
	if (split[0] == NULL || split[1] == NULL || split[2] == NULL || \
	split[3] == NULL || split[4] != NULL)
		return (free_split(split));
	if (split[0][0] != 'L' && split[0][1] != '\0')
		return (free_split(split));
	if (xyz_check(split[1]) == 0)
		return (free_split(split));
	if (pos_decimal_check(split[2]) == 0)
		return (free_split(split));
	if (rgb_check(split[3], 0, 255) == 0)
		return (free_split(split));
	if (setup_light(split, map) == 0)
		return (free_split(split));
	free_split(split);
	element_count->light++;
	return (1);
}
