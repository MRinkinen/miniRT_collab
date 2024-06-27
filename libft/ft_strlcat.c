/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:46:19 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:48:06 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	char			*temp_dst_ptr;
	const char		*temp_dst_src;
	unsigned long	n;
	unsigned long	dst_lenght;

	temp_dst_ptr = dst;
	temp_dst_src = src;
	n = dstsize;
	while (n-- != 0 && *temp_dst_ptr != '\0')
		temp_dst_ptr++;
	dst_lenght = temp_dst_ptr - dst;
	n = dstsize - dst_lenght;
	if (n == 0)
		return (dst_lenght + ft_strlen(temp_dst_src));
	while (*temp_dst_src != '\0')
	{
		if (n != 1)
		{
			*temp_dst_ptr++ = *temp_dst_src;
			n--;
		}
		temp_dst_src++;
	}
	*temp_dst_ptr = '\0';
	return (dst_lenght + (temp_dst_src - src));
}
