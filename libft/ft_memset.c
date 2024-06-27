/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:23:50 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:24:01 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned long	i;
	char			*temp_ptr;

	temp_ptr = (char *)b;
	i = 0;
	while (i < len)
	{
		temp_ptr[i] = c;
		i++;
	}
	return (temp_ptr);
}
