/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:22:39 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 11:49:05 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	loop(unsigned char *dst, const unsigned char *src, size_t len)
{
	unsigned long	i;

	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*temp_ptr_dst;
	const unsigned char	*temp_ptr_src;

	temp_ptr_dst = (unsigned char *)dst;
	temp_ptr_src = (const unsigned char *)src;
	if (dst == NULL && src == NULL)
		return (NULL);
	if (temp_ptr_src < temp_ptr_dst)
	{
		while (len > 0)
		{
			temp_ptr_dst[len - 1] = temp_ptr_src[len - 1];
			len--;
		}
	}
	else
		loop(temp_ptr_dst, temp_ptr_src, len);
	return (temp_ptr_dst);
}
