/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:58:10 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/14 11:48:14 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	setchecker(char chr, char *set, int setlenght)
{
	int	i;

	i = 0;
	while (i < setlenght)
	{
		if (chr == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	setstart(char *str, char *set)
{
	int	i;
	int	str_lenght;
	int	set_lenght;

	str_lenght = ft_strlen(str);
	set_lenght = ft_strlen(set);
	i = 0;
	while (i < str_lenght)
	{
		if (setchecker(str[i], set, set_lenght) == 1)
			i++;
		else
			break ;
	}
	return (i);
}

static int	setend(char *str, char *set, unsigned int start)
{
	int	i;
	int	str_lenght;
	int	set_lenght;

	str_lenght = ft_strlen(str);
	set_lenght = ft_strlen(set);
	i = str_lenght - 1;
	while (i > (int)start)
	{
		if (setchecker(str[i], set, set_lenght) == 1)
			i--;
		else
			break ;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	x;
	unsigned int	y;
	char			*temp_str;

	i = setstart((char *)s1, (char *)set);
	x = setend((char *)s1, (char *)set, i);
	y = 0;
	if (ft_strlen(s1) != 0)
	{
		temp_str = (char *)malloc(x - i + 2);
		if (!temp_str)
			return (NULL);
		while (i <= x)
			temp_str[y++] = s1[i++];
	}
	else
	{
		temp_str = (char *)malloc(1);
		if (!temp_str)
			return (NULL);
	}
	temp_str[y] = '\0';
	return (temp_str);
}
