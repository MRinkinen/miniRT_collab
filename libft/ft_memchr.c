/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:17:26 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/14 11:43:24 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned long	i;
	char			*temp_ptr;

	temp_ptr = (char *)s;
	i = 0;
	while (i < n)
	{
		if ((unsigned char)temp_ptr[i] == (unsigned char)c)
			return ((void *)temp_ptr + i);
		i++;
	}
	return (NULL);
}
