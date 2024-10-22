/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 19:18:40 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/22 12:21:35 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"
#include <ctype.h>

int is_rgb_number(char **split)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 3)
	{
		while (split[i][j] != '\0')
		{
			if (split[i][j] == '\n')
				break ;
			if (!ft_isdigit(split[i][j]))
				return (0);
			j++;
		}
		j = 0;
		i++;
	}
	return (1);
}

int	rgb_check(char *rgb, int min, int max)
{
	char	**split;

	split = ft_split(rgb, ',');
	if (split[0] == NULL)
		return (free_split(split));
	if (split[1] == NULL)
		return (free_split(split));
	if (split[2] == NULL)
		return (free_split(split));
	if (split[3] != NULL)
		return (free_split(split));
	if (is_rgb_number(split) == 0)
		return (free_split(split));
	if (ft_atoi(split[0]) < min || ft_atoi(split[0]) > max)
		return (free_split(split));
	if (ft_atoi(split[1]) < min || ft_atoi(split[1]) > max)
		return (free_split(split));
	if (ft_atoi(split[2]) < min || ft_atoi(split[2]) > max)
		return (free_split(split));
	free_split(split);
	return (1);
}

int pos_decimal_check(char *str)
{
	double	value;
	char	*c;

	c = str;
	while (*c)
	{
		if (!ft_isdigit(*c) && *c != '.')
			return (0);
		c++;
	}
	value = ft_atof(str);
	c = str;
	if (*c == '-' || *c == '+')
		return (0);
	while (ft_isdigit(*c))
		c++;
	if (*c == '.')
		c++;
	while (ft_isdigit(*c))
		c++;
	if (*c != '\0' || value < 0.0 || value > 1.0)
		return (0);
	return (1);
}

int	check_element_count(t_element_count *element_count, int flag)
{
	if (element_count->ambient > 1)
		return (0);
	if (element_count->camera > 1)
		return (0);
	if (element_count->light > 1)
		return (0);
	if (element_count->sphere > 20)
		return (0);
	if (element_count->plane > 10)
		return (0);
	if (element_count->cylinder > 10)
		return (0);
	if (flag == 1)
	{
		if (element_count->ambient == 0 || element_count->camera == 0)
			return (0);
		if (element_count->light == 0 || element_count->sphere == 0)
			return (0);
		if (element_count->plane == 0 || element_count->cylinder == 0)
			return (0);
	}
	return (1);
}

int	validate_lines(char *line, t_element_count *element_count, t_map *map)
{
	if (check_element_count(element_count, 0) == 0)
		return (0);
	else if (ft_strncmp(line, "\n", 1) == 0)
		return (1);
	else if (ft_strncmp(line, "A", 1) == 0)
		return (validate_ambient(line, element_count, map));
	else if (ft_strncmp(line, "C", 1) == 0)
		return (validate_camera(line, element_count, map));
	else if (ft_strncmp(line, "L", 1) == 0)
		return (validate_light(line, element_count, map));
	else if (ft_strncmp(line, "sp", 2) == 0)
		return (validate_sphere(line, element_count, map));
	else if (ft_strncmp(line, "pl", 2) == 0)
		return (validate_plane(line, element_count, map));
	else if (ft_strncmp(line, "cy", 2) == 0)
		return (validate_cylinder(line, element_count, map));
	else
		return (0);
	if (check_element_count(element_count, 1) == 0)
		return (0);
	return (1);
}
