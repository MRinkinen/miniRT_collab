/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:39:45 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/14 11:42:27 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int			i;
	int			x;
	const char	*temp_ptr;

	temp_ptr = s;
	i = 0;
	x = ft_strlen(s) + 1;
	while (i < x)
	{
		if ((unsigned char)temp_ptr[i] == (unsigned char)c)
			return ((char *)temp_ptr + i);
		i++;
	}
	return (0);
}
