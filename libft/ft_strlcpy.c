/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:07:40 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/07 15:45:21 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	unsigned long	size;
	const char		*temp_ptr_src;

	temp_ptr_src = src;
	size = dstsize;
	if (size != 0 && --size != 0)
	{
		while (size != 0)
		{
			*dst = *temp_ptr_src;
			if (temp_ptr_src++ == 0 || *dst++ == 0)
				break ;
			size--;
		}
	}
	if (size == 0)
	{
		if (dstsize != 0)
			*dst = '\0';
		while (*temp_ptr_src++)
			;
	}
	return (temp_ptr_src - src - 1);
}
