/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:26:47 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:32:25 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/parsing_bonus.h"

int	check_filename(char *file)
{
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd != -1)
		return (fd);
	return (-1);
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
