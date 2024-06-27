/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:47:38 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/15 13:42:34 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countstrings(char *str, char c)
{
	int	charcounter;
	int	ischaraused;

	charcounter = 0;
	ischaraused = 0;
	while (*str)
	{
		if (*str != c)
		{
			if (!ischaraused)
			{
				ischaraused = 1;
				charcounter++;
			}
		}
		else
			ischaraused = 0;
		str++;
	}
	return (charcounter);
}

static char	**loop(char **tempstr, char const *s, char c, int stringcount)
{
	int			i;
	const char	*start;

	i = 0;
	while (*s && i <= stringcount)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			start = s;
			while (*s && *s != c)
				s++;
			tempstr[i] = ft_substr(start, 0, s - start);
			if (!tempstr[i])
			{
				while (--i >= 0)
					free(tempstr[i]);
				free(tempstr);
				return (NULL);
			}
			i++;
		}
	}
	return (tempstr);
}

char	**ft_split(char const *s, char c)
{
	int		stringcount;
	char	**tempstr;

	stringcount = countstrings((char *)s, c);
	tempstr = (char **)malloc((stringcount + 1) * sizeof(char *));
	if (!tempstr)
		return (NULL);
	tempstr = loop(tempstr, s, c, stringcount);
	if (!tempstr)
		return (NULL);
	tempstr[stringcount] = NULL;
	return (tempstr);
}
