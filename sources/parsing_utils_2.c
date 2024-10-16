/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 11:19:13 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/15 18:34:20 by tvalimak         ###   ########.fr       */
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

int decimal_check(char *str, int min, int max)
{
    double  value;
    char    *c;

    c = str;
    while (*c)
    {
        if (!ft_isdigit(*c) && *c != '.' && *c != '-')
        {
            printf("returning 0 from decimal check 0\n");
            return 0;
        }
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
    {
        printf("returning 0 from decimal check 1\n");
        return 0;
    }
    printf("returned 1 from decimal check\n");
    return 1;
}

// range for coordinates x,y,z are -10000 to 10000
int xyz_check(char *str)
{
    char    **split;
    int     i;

    i = 0;
    while (str[i + 1] != '\0')
        i++;
    if (str[i] == ',')
    {
        printf("returning 0 from xyz check -1\n");
        return (0);
    }
    split = ft_split(str, ',');
    if (!split[0] || !split[1] || !split[2] || split[3])
    {
        printf("returning 0 from xyz check 0\n");
        return (free_split(split));
    }
    printf("%s\n%s\n%s\n%s", split[0], split[1], split[2], split[3]);
    i = 0;
    while (split[i])
    {
        if (decimal_check(split[i], -10000, 10000) == 0)
        {
            printf("returning 0 from xyz check 1\n");
            return (free_split(split));
        }
        i++;
    }
    free_split(split);
    printf("returning 1 from xyz check\n");
    return (1);
}

int degree_check(char *str, int min, int max)
{
    if (ft_atoi(str) < min || ft_atoi(str) > max)
    {
        printf("returning 0 from degree_check 0\n");
        return (0);
    }
    printf("returning 1 from degree check\n");
    return (1);
}
// 3d normalized orientation vector. In range [-1,1] for each x,y,z axis:
// 0.0,0.0,1.0

int vectors_check(char *str)
{
    int i;
    char **split;
    double x, y, z;

    i = 0;
    while (str[i + 1] != '\0')
        i++;
    if (str[i] == ',')
    {
        printf("returning 0 from xyz check -1\n");
        return (0);
    }
    split = ft_split(str, ',');
    if (!split[0] || !split[1] || !split[2] || split[3])
    {
        printf("returning 0 from vectors check 0\n");
        return (free_split(split));
    }
    x = ft_atof(split[0]);
    y = ft_atof(split[1]);
    z = ft_atof(split[2]);
    // Check if all vector coordinates are zero
    if (x == 0.0 && y == 0.0 && z == 0.0)
    {
        printf("returning 0 from vectors check (zero vector)\n");
        return (free_split(split));
    }
    i = 0;
    while (split[i])
    {
        if (decimal_check(split[i], -1, 1) == 0)
        {
            printf("returning 0 from vectors check 1\n");
            return (free_split(split));
        }
        i++;
    }
    free_split(split);
    printf("returning 1 from vectors check\n");
    return (1);
}
