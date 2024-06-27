/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:52:14 by mrinkine          #+#    #+#             */
/*   Updated: 2023/12/18 11:02:21 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*readinbuffer(int fd, char *ret_line)
{
	char	*temp;
	ssize_t	charsread;
	size_t	temp_lenght;

	temp = malloc((sizeof(char) * (BUFFER_SIZE + 1)));
	if (!temp)
		return (return_and_free(NULL, ret_line, 0));
	while (1)
	{
		charsread = read(fd, temp, BUFFER_SIZE);
		if (charsread <= 0 && !ret_line)
			return (return_and_free(NULL, temp, 0));
		if (charsread <= 0)
			return (return_and_free(ret_line, temp, 0));
		temp[charsread] = '\0';
		temp_lenght = gnl_stringlenght(temp, 0);
		ret_line = gnl_strjoin(ret_line, temp, 0);
		if (!ret_line)
			return (return_and_free(NULL, temp, 0));
		if (temp_lenght < BUFFER_SIZE || charsread < BUFFER_SIZE)
			break ;
	}
	return (return_and_free(ret_line, temp, 0));
}

int	retline_check(char *ret_line, void *s)
{
	char	*static_string;

	static_string = (char *)s;
	if (gnl_stringlenght(ret_line, 0) == gnl_stringlenght(ret_line, 1))
	{
		gnl_bzero(static_string, BUFFER_SIZE);
	}
	if (gnl_stringlenght(ret_line, 0) < gnl_stringlenght(ret_line, 1))
	{
		gnl_bzero(static_string, BUFFER_SIZE);
		gnl_cleanstring(static_string, ret_line);
	}
	return (1);
}

char	*mainloop(char *ret_line, char *static_string, size_t stat_len, int fd)
{
	while (1)
	{
		if (stat_len > 0)
		{
			ret_line = gnl_strjoin(ret_line, static_string, 0);
			if (!ret_line)
				return (NULL);
			if (gnl_stringlenght(static_string, 0) < stat_len)
			{
				ret_line = gnl_strjoin(ret_line, static_string, 0);
				if (!ret_line)
					return (NULL);
				gnl_cleanstring(static_string, static_string);
				return (ret_line);
			}
		}
		ret_line = readinbuffer(fd, ret_line);
		if (!ret_line)
			return (NULL);
		if (retline_check(ret_line, static_string) == 1)
			return (ret_line);
	}
}

char	*cleanstringbeforereturn(char *ret_line)
{
	int		i;
	char	*final_return_line;

	final_return_line = NULL;
	if (!ret_line)
		return (NULL);
	i = 0;
	while (ret_line[i] != '\0')
	{
		if (ret_line[i] == '\n')
			ret_line[i + 1] = '\0';
		i++;
	}
	final_return_line = malloc(i + 1);
	if (!final_return_line)
		return (free(ret_line), NULL);
	i = 0;
	while (ret_line[i] != '\0')
	{
		final_return_line[i] = ret_line[i];
		i++;
	}
	free(ret_line);
	final_return_line[i] = '\0';
	return (final_return_line);
}

char	*get_next_line(int fd)
{
	static char		static_string[65534][BUFFER_SIZE];
	char			*ret_line;
	size_t			static_string_lenght;

	ret_line = NULL;
	if (fd < 0 || fd > 65534)
		return (NULL);
	static_string_lenght = gnl_stringlenght(static_string[fd], 1);
	if (BUFFER_SIZE <= 0 || read(fd, ret_line, 0) < 0)
		return (return_and_free(NULL, static_string[fd], 1));
	ret_line = mainloop(ret_line, static_string[fd], static_string_lenght, fd);
	if (!ret_line)
	{
		return (return_and_free(NULL, static_string[fd], 1));
	}
	ret_line = cleanstringbeforereturn(ret_line);
	if (!ret_line)
	{
		ret_line = NULL;
		return (return_and_free(NULL, static_string[fd], 1));
	}
	return (ret_line);
}
