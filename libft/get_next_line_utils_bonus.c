/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:51:02 by mrinkine          #+#    #+#             */
/*   Updated: 2023/12/18 10:59:49 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	gnl_stringlenght(char *s, int line_or_end)
{
	size_t	i;

	i = 0;
	if (line_or_end == 0)
	{
		while (s[i] && s[i] != '\n')
			i++;
	}
	else
	{
		if (!s)
			return (0);
		while (s[i])
			i++;
	}
	return (i);
}

char	*gnl_strjoin(char *s1, char *s2, int frees2)
{
	size_t	i;
	size_t	temp_length;
	size_t	s1_lenght;
	char	*temp;

	i = 0;
	s1_lenght = gnl_stringlenght(s1, 1);
	temp_length = s1_lenght + gnl_stringlenght(s2, 1);
	temp = malloc(sizeof(char) * (temp_length + 1));
	if (!temp)
		return (return_and_free(NULL, s1, 0));
	while (i < s1_lenght && s1[i] != '\0')
	{
		temp[i] = s1[i];
		i++;
	}
	while (i < temp_length)
	{
		temp[i] = s2[i - s1_lenght];
		i++;
	}
	if (frees2)
		free(s2);
	temp[temp_length] = '\0';
	return (return_and_free(temp, s1, 0));
}

void	gnl_cleanstring(char *dst, const char *src)
{
	const char	*temp_ptr;

	temp_ptr = src;
	while (*temp_ptr != '\0' && *temp_ptr != '\n' && *temp_ptr != -1)
	{
		temp_ptr++;
	}
	temp_ptr++;
	while (*temp_ptr)
	{
		*dst = *temp_ptr;
		if (temp_ptr++ == 0 || *dst++ == 0)
			break ;
	}
	*dst = '\0';
}

void	gnl_bzero(void *s, size_t n)
{
	unsigned long	i;
	char			*x;

	x = (char *)s;
	i = 0;
	while (i < n)
	{
		x[i] = '\0';
		i++;
	}
}

char	*return_and_free(char *ret, char *string_to_free, int zero)
{
	if (zero)
		gnl_bzero(string_to_free, BUFFER_SIZE);
	else
	{
		if (string_to_free)
		{
			free(string_to_free);
			string_to_free = NULL;
		}
	}
	if (ret == NULL)
		return (NULL);
	return (ret);
}
