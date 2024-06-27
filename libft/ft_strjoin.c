/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:44:29 by mrinkine          #+#    #+#             */
/*   Updated: 2023/11/06 10:44:47 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		temp_length;
	char	*temp;
	int		s1_lenght;
	int		s2_lenght;

	i = 0;
	s1_lenght = ft_strlen(s1);
	s2_lenght = ft_strlen(s2);
	temp_length = s1_lenght + s2_lenght;
	temp = (char *)malloc(temp_length + 1);
	if (!temp)
		return (NULL);
	while (i < s1_lenght)
	{
		temp[i] = s1[i];
		i++;
	}
	while (i < temp_length)
	{
		temp[i] = s2[i - s1_lenght];
		i++;
	}
	temp[temp_length] = '\0';
	return (temp);
}
