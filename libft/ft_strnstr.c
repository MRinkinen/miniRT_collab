/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:55:47 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/14 11:51:07 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	unsigned long	i;
	unsigned long	y;
	unsigned long	needlelen;

	i = 0;
	y = 0;
	needlelen = ft_strlen(needle);
	if (needlelen == 0 || haystack == needle)
		return ((char *)haystack);
	while (i < len && haystack[i] != '\0')
	{
		while (i + y < len && haystack[i + y] != '\0'
			&& needle[y] != '\0' && haystack[i + y] == needle[y])
			y++;
		if (y == needlelen)
			return ((char *)haystack + i);
		y = 0;
		i++;
	}
	return (NULL);
}
