/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:12:20 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:15:25 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countdigits(int number)
{
	int	count;

	count = 0;
	while (number != 0)
	{
		number = number / 10;
		++count;
	}
	return (count);
}

static void	numbertostring(int digits, int n, char *number, int isnegative)
{
	int	i;

	i = digits + isnegative;
	while (n != 0)
	{
		number[--i] = '0' + (n % 10);
		n /= 10;
	}
	number[digits + isnegative] = '\0';
}

static char	*hanleintminandzero(int n, char *number)
{
	if (n == -2147483648)
	{
		number = (char *)malloc(12);
		if (!number)
			return (NULL);
		ft_strlcpy(number, "-2147483648", 12);
	}
	if (n == -0)
	{
		number = (char *)malloc(2);
		if (!number)
			return (NULL);
		number[0] = '0';
		number[1] = '\0';
	}
	return (number);
}

char	*ft_itoa(int n)
{
	int		digits;
	int		isnegative;
	char	*number;

	if (n == -2147483648 || n == -0)
	{
		number = NULL;
		number = hanleintminandzero(n, number);
		return (number);
	}
	isnegative = (n < 0);
	digits = countdigits(n);
	number = (char *)malloc(digits + isnegative + 1);
	if (!number)
		return (NULL);
	if (isnegative)
	{
		number[0] = '-';
		n = n * -1;
	}
	numbertostring(digits, n, number, isnegative);
	return (number);
}
