/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:01:02 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/15 17:46:55 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void			*temp_ptr;
	unsigned long	limit;

	limit = 0 - 1;
	if (count != 0 && size != 0 && (limit / count < size))
		return (NULL);
	if (count * size > 2147483424)
		return (NULL);
	if (count == 0 || size == 0)
		return (malloc(0));
	temp_ptr = malloc(size * count);
	if (!temp_ptr)
		return (NULL);
	ft_memset(temp_ptr, 0, count * size);
	return (temp_ptr);
}
