/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_three_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:55:07 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:34:25 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

double	ft_atof(const char *str)
{
	double	res;
	double	res2;
	int		len;
	int		sign;
	char	*c;

	c = (char *)str;
	sign = 1;
	if (*c == '-')
	{
		sign = -1;
		c++;
	}
	else if (*c == '+')
		c++;
	res = (double)ft_atoi(c);
	while (*c && *c != '.')
		c++;
	if (*c == '.')
		c++;
	res2 = (double)ft_atoi(c);
	len = ft_strlen(c);
	while (len--)
		res2 /= 10;
	return (sign * (res + res2));
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
