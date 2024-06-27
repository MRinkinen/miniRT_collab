/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:18:51 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:19:06 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned long		i;
	const unsigned char	*temp_ptr_s1;
	const unsigned char	*temp_ptr_s2;

	temp_ptr_s1 = (const unsigned char *)s1;
	temp_ptr_s2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (temp_ptr_s1[i] != temp_ptr_s2[i])
			return (temp_ptr_s1[i] - temp_ptr_s2[i]);
		i++;
	}
	return (0);
}
