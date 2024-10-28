/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_camera.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:05:28 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 18:17:04 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	validate_camera(char *line, t_element_count *element_count, \
	t_map	*map)
{
	char	**split;

	replace_tabs_with_spaces(line);
	split = ft_split(line, ' ');
	if (split[0][0] != 'C' || split[0][1] != '\0')
		return (free_split(split));
	if (split[1] == NULL || split[2] == NULL || \
	split[3] == NULL || split[4] != NULL)
		return (free_split(split));
	if (xyz_check(split[1]) == 0)
		return (free_split(split));
	if (vectors_check(split[2]) == 0)
		return (free_split(split));
	if (degree_check(split[3], 0, 180) == 0)
		return (free_split(split));
	if (setup_camera(split, map) == 0)
		return (free_split(split));
	free_split(split);
	element_count->camera++;
	return (1);
}
