/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_three.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:55:07 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 18:55:40 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
