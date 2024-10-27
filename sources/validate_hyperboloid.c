/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_hyperboloid.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:04:34 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/27 18:54:58 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	validate_hyperboloid(char *line, t_element_count *element_count, \
	t_map *map)
{
	char	**split;

	printf("in validate hyperboloid\n");
	replace_tabs_with_spaces(line);
	split = ft_split(line, ' ');
	printf("in validate hyperboloid 0\n");
	if (!split[1] || !split[2] || !split[3] || \
	!split[4] || !split[5] || split[6])
		return (free_split(split));
	printf("in validate hyperboloid 1\n");
	if (split[0][2] != '\0')
		return (free_split(split));
	printf("in validate hyperboloid 2\n");
	if (xyz_check(split[1]) == 0)
		return (free_split(split));
	printf("in validate hyperboloid 3\n");
	if (vectors_check(split[2]) == 0)
		return (free_split(split));
	printf("in validate hyperboloid 4\n");
	if (xyz_check(split[3]) == 0)
		return (free_split(split));
	printf("in validate hyperboloid 5\n");
	if (decimal_check(split[4], 0, 100) == 0)
		return (free_split(split));
	printf("in validate hyperboloid 6\n");
	if (rgb_check(split[5], 0, 256) == 0)
		return (free_split(split));
	printf("in validate hyperboloid 7\n");
	if (setup_cylinder(split, map) == 0)
		return (free_split(split));
	printf("in validate hyperboloid 8\n");
	free_split(split);
	element_count->hyperboloid++;
	printf("validate hyperboloid ended successfully\n");
	return (1);
}
