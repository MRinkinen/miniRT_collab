/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writechar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:56:52 by mrinkine          #+#    #+#             */
/*   Updated: 2023/12/18 10:57:34 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_print(char *s)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (s == NULL)
	{
		i = ft_putstr_print("(null)");
	}
	else
	{
		while (*s)
		{
			count = write(1, s, 1);
			if (count < 0)
				return (-1);
			else
				i += count;
			s++;
		}
	}
	return (i);
}

int	ft_putchar_print(int c)
{
	int	i;

	i = write(1, &c, 1);
	if (i < 0)
		return (-1);
	return (i);
}
