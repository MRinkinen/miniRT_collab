/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writenumber.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:57:40 by mrinkine          #+#    #+#             */
/*   Updated: 2023/12/18 10:58:13 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_print(long n, int count)
{
	char	number[12];
	int		i;

	if (n == 0)
		count += ft_putstr_print("0");
	else
	{
		i = 10;
		if (n < 0)
		{
			count += ft_putchar_print('-');
			if (count < 0)
				return (-1);
			n = -n;
		}
		number[11] = '\0';
		while (n > 0)
		{
			number[i] = '0' + (n % 10);
			n /= 10;
			i--;
		}
		count += ft_putstr_print(number + i + 1);
	}
	return (count);
}

int	ft_puthex_print_x(unsigned long long hex, int i)
{
	if (i < 0)
		return (-1);
	if (hex > 15)
	{
		i = ft_puthex_print_x(hex / 16, i);
		i = ft_puthex_print_x(hex % 16, i);
	}
	else
	{
		if (hex < 10)
			i += ft_putchar_print(hex + 48);
		else
			i += ft_putchar_print(hex - 10 + 'a');
	}
	return (i);
}

int	ft_puthex_print_xx(unsigned int hex, int i)
{
	if (i < 0)
		return (-1);
	if (hex > 15)
	{
		i = ft_puthex_print_xx(hex / 16, i);
		i = ft_puthex_print_xx(hex % 16, i);
	}
	else
	{
		if (hex < 10)
			i += ft_putchar_print(hex + 48);
		else
			i += ft_putchar_print(hex - 10 + 'A');
	}
	return (i);
}

int	ft_putvoidptr_print(unsigned long long ptr)
{
	int	i;

	i = write(1, "0x", 2);
	if (i < 0)
		return (-1);
	if (ptr == 0)
		i += ft_putchar_print('0');
	else
		i += ft_puthex_print_x(ptr, 0);
	return (i);
}
