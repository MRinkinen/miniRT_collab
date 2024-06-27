/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:02:58 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/14 11:38:05 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	unsigned long	s_lenght;
	char			*temp_str;

	s_lenght = ft_strlen(s);
	i = 0;
	if ((size_t)start >= ft_strlen(s))
		return (ft_strdup(""));
	if (start >= s_lenght)
		return ((char *)s);
	if (len > (s_lenght - start))
		len = s_lenght - start;
	temp_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!temp_str)
		return (NULL);
	while (i < len)
	{
		temp_str[i] = s[start];
		start++;
		i++;
	}
	temp_str[i] = '\0';
	return (temp_str);
}
