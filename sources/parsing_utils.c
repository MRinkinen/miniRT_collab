/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 19:18:40 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/08 15:55:14 by tvalimak         ###   ########.fr       */
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
	printf("%s\n%s\n%s", split[0] , split[1], split[2]);
	while (i < 3)
	{
		printf("inside is rgb number while loop\n");
		while (split[i][j] != '\0')
		{
			if (split[i][j] == '\n')
				break ;
			printf("inside is rgb number while loop 2\n");
			printf("split[i][j] = %c\n", split[i][j]);
			if (!ft_isdigit(split[i][j]))
			{
				printf("returning 0 from is rgb number\n");
				return (0);
			}
			j++;
		}
		j = 0;
		i++;
	}
	printf("returned 1 from is rgb number\n");
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
	printf("inside rgb check 0\n");
	if (is_rgb_number(split) == 0)
		return (free_split(split));
	printf("inside rgb check 1\n");
	if (ft_atoi(split[0]) < min || ft_atoi(split[0]) > max)
	{
		printf("returning 0 from rgb check 0\n");
		return (free_split(split));
	}
	if (ft_atoi(split[1]) < min || ft_atoi(split[1]) > max)
	{
		printf("returning 0 from rgb check 1\n");
		return (free_split(split));
	}
	if (ft_atoi(split[2]) < min || ft_atoi(split[2]) > max)
	{
		printf("returning 0 from rgb check 2\n");
		return (free_split(split));
	}
	free_split(split);
	printf("returned 1 from rgb check\n");
	return (1);
}

int pos_decimal_check(char *str) 
{
    double value;
    char *c = str;

    while (*c)
	{
        if (!ft_isdigit(*c) && *c != '.')
		{
            printf("returning 0 from pos decimal check 0\n");
            return 0;
        }
        c++;
    }
    value = ft_atof(str);
    c = str;
    if (*c == '-' || *c == '+')
	{
		printf("returning 0 from pos decimal check 1\n");
		return (0);
    }
    while (ft_isdigit(*c))
        c++;
    if (*c == '.')
        c++;
    while (ft_isdigit(*c))
        c++;
    if (*c != '\0' || value < 0.0 || value > 1.0)
	{
        printf("returning 0 from pos decimal check 2\n");
        return 0;
    }
    printf("returned 1 from pos decimal check\n");
    return 1;
}

int	check_element_count(t_element_count *element_count, int flag)
{
	if (element_count->ambient > 1)
		return (0);
	if (element_count->camera > 1)
		return (0);
	if (element_count->light > 3)
		return (0);
	if (element_count->sphere > 10)
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
	printf("returned 1 from check element count\n");
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
