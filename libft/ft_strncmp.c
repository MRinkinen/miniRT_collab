/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:54:10 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:54:28 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int		i;
	unsigned int		r;
	const unsigned char	*temp_ptr_s1;
	const unsigned char	*temp_ptr_s2;

	temp_ptr_s1 = (const unsigned char *)s1;
	temp_ptr_s2 = (const unsigned char *)s2;
	i = 0;
	while ((temp_ptr_s1[i] != '\0' || temp_ptr_s2[i] != '\0') && i < n)
	{
		if (temp_ptr_s1[i] == temp_ptr_s2[i])
			i++;
		else
		{
			r = temp_ptr_s1[i] - temp_ptr_s2[i];
			return (r);
		}
	}
	return (0);
}
