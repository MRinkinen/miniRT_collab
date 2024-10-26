/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 11:19:13 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 18:49:31 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

#include "../includes/parsing.h"

double	ft_atof(const char *str)
{
	double	res;
	double	res2;
	char	*c;
	int		len;

	c = (char *)str;
	res = (double)ft_atoi(c);
	while (*c && *c != '.')
		c++;
	if (*c == '.')
		c++;
	res2 = (double)ft_atoi(c);
	len = ft_strlen(c);
	while (len--)
		res2 /= 10;
	if (res >= 0)
		return (res + res2);
	else
		return (res + -res2);
}

int	decimal_check(char *str, int min, int max)
{
	double	value;
	char	*c;

	c = str;
	while (*c)
	{
		if (!ft_isdigit(*c) && *c != '.' && *c != '-')
			return (0);
		c++;
	}
	value = ft_atof(str);
	if (*c == '-')
		c++;
	while (ft_isdigit(*c))
		c++;
	if (*c == '.')
		c++;
	while (ft_isdigit(*c))
		c++;
	if (*c != '\0' || value < min || value > max)
		return (0);
	return (1);
}

int	xyz_check(char *str)
{
	char	**split;
	int		i;

	i = 0;
	while (str[i + 1] != '\0')
		i++;
	if (str[i] == ',')
		return (0);
	split = ft_split(str, ',');
	if (!split[0] || !split[1] || !split[2] || split[3])
		return (free_split(split));
	i = 0;
	while (split[i])
	{
		if (decimal_check(split[i], -10000, 10000) == 0)
			return (free_split(split));
		i++;
	}
	free_split(split);
	return (1);
}

int	degree_check(char *str, int min, int max)
{
	if (ft_atoi(str) < min || ft_atoi(str) > max)
		return (0);
	return (1);
}

int	validate_vector_values(char **split)
{
	double	x;
	double	y;
	double	z;
	int		i;

	x = ft_atof(split[0]);
	y = ft_atof(split[1]);
	z = ft_atof(split[2]);

	if (x == 0.0 && y == 0.0 && z == 0.0)
		return (free_split(split));

	i = 0;
	while (split[i])
	{
		if (decimal_check(split[i], -1, 1) == 0)
			return (free_split(split));
		i++;
	}
	return (1);
}

int	vectors_check(char *str)
{
	int		len;
	char	**split;

	len = ft_strlen(str);
	if (str[len - 1] == ',')
		return (0);

	split = ft_split(str, ',');
	if (!split[0] || !split[1] || !split[2] || split[3])
		return (free_split(split));

	if (!validate_vector_values(split))
		return (0);

	free_split(split);
	return (1);
}
