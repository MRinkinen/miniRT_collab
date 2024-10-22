/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_ambient.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 19:16:31 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/22 13:10:36 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	replace_tabs_with_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\t')
			line[i] = ' ';
		i++;
	}
}

int	validate_ambient(char *line, t_element_count *element_count, t_map *map)
{
	char	**split;

	replace_tabs_with_spaces(line);
	split = ft_split(line, ' ');
	if (split[0][0] != 'A' || split[0][1] != '\0')
		return (free_split(split));
	if (split[1] == NULL || split[2] == NULL || split[3] != NULL)
		return (free_split(split));
	if (pos_decimal_check(split[1]) == 0)
		return (free_split(split));
	if (rgb_check(split[2], 0, 255) == 0)
		return (free_split(split));
	if (setup_ambient(split, map) == 0)
		return (free_split(split));
	free_split(split);
	element_count->ambient++;
	return (1);
}
