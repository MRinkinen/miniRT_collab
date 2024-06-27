/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:53:05 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:53:18 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	int		strlenght;
	char	*temp;

	i = 0;
	strlenght = ft_strlen(s);
	temp = (char *)malloc(strlenght + 1);
	if (!temp)
		return (NULL);
	while (s[i] != '\0')
	{
		temp[i] = f(i, s[i]);
		i++;
	}
	temp[strlenght] = '\0';
	return (temp);
}
