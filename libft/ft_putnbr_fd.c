/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:26:18 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:28:09 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	number[12];
	int		i;

	if (n == 0)
		ft_putstr_fd("0", fd);
	else if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else
	{
		i = 10;
		if (n < 0)
		{
			ft_putchar_fd('-', fd);
			n = -n;
		}
		number[11] = '\0';
		while (n > 0)
		{
			number[i] = '0' + (n % 10);
			n /= 10;
			i--;
		}
		ft_putstr_fd(number + i + 1, fd);
	}
}
