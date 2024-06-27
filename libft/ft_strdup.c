/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:40:40 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:40:54 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*temp;
	int		lenght;

	lenght = ft_strlen(s1);
	temp = (char *)malloc(lenght + 1);
	if (!temp)
		return (NULL);
	ft_strlcpy(temp, s1, lenght + 1);
	temp[lenght] = '\0';
	return (temp);
}
