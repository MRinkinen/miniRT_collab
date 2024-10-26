/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:26:47 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 19:03:12 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	check_filename(char *file)
{
	char	*temp;
	char	*path;
	int		fd;

	temp = ft_strjoin("./maps/", file);
	path = ft_strjoin(temp, ".rt");
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		free(temp);
		free(path);
		printf("Invalid filename or error in open\n");
		return (-1);
	}
	free(temp);
	free(path);
	return (fd);
}

int	read_to_parse(t_element_count *element_count, t_map *map, char **file)
{
	int		fd;
	char	*line;

	fd = check_filename(file[1]);
	if (fd == -1)
		return (0);
	line = get_next_line(fd);
	while (line)
	{
		if (validate_lines(line, element_count, map) == 0)
		{
			free(line);
			printf("Invalid file\n");
			close(fd);
			return (0);
		}
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (1);
}
