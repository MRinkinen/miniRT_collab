/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:19:58 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:20:16 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned long	i;
	char			*temp_ptr_dst;
	char			*temp_ptr_src;

	temp_ptr_dst = (char *)dst;
	temp_ptr_src = (char *)src;
	i = 0;
	if (dst == NULL && src == NULL && n > 0)
		return (temp_ptr_src);
	while (i < n)
	{
		temp_ptr_dst[i] = temp_ptr_src[i];
		i++;
	}
	return (temp_ptr_dst);
}
