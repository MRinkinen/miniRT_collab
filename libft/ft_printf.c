/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:44:10 by mrinkine          #+#    #+#             */
/*   Updated: 2023/12/18 11:28:10 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	printconversion(va_list list, char c)
{
	int	count;

	if (c == 's')
		count = ft_putstr_print(va_arg(list, char *));
	else if (c == 'c')
		count = ft_putchar_print(va_arg(list, int));
	else if (c == 'i')
		count = ft_putnbr_print(va_arg(list, int), 0);
	else if (c == '%')
		count = ft_putchar_print('%');
	else if (c == 'p')
		count = ft_putvoidptr_print(va_arg(list, unsigned long));
	else if (c == 'd')
		count = ft_putnbr_print(va_arg(list, int), 0);
	else if (c == 'u')
		count = ft_putnbr_print(va_arg(list, unsigned int), 0);
	else if (c == 'x')
		count = ft_puthex_print_x(va_arg(list, unsigned int), 0);
	else if (c == 'X')
		count = ft_puthex_print_xx(va_arg(list, unsigned int), 0);
	else
		count = ft_putchar_print(c);
	return (count);
}

int	printstring(char *str, va_list list)
{
	int	charcounter;
	int	temp_int;

	charcounter = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			temp_int = printconversion(list, *str);
		}
		else
		{
			temp_int = write(1, str, 1);
		}
		if (temp_int == -1)
			return (-1);
		else
			charcounter += temp_int;
		str++;
	}
	return (charcounter);
}

int	ft_printf(const char *str, ...)
{
	va_list	argptr;
	int		conversions;

	va_start(argptr, str);
	conversions = printstring((char *)str, argptr);
	va_end(argptr);
	return (conversions);
}
