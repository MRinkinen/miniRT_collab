/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_camera.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:05:28 by tvalimak          #+#    #+#             */
/*   Updated: 2024/08/20 13:50:28 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

// Camera data parser has to handle:
/*
∗ identifier: C
∗ x,y,z coordinates of the view point: -50.0,0,20
∗ 3d normalized orientation vector. In range [-1,1] for each x,y,z axis: 0.0,0.0,1.0
∗ FOV : Horizontal field of view in degrees in range [0,180]: 70*/

int	validate_camera(char *line, t_element_count *element_count, \
    t_map *map)
{
    char	**split;

    printf("inside validate_camera\n");
	split = ft_split(line, ' ');
	printf("%s\n%s\n%s\n%s", split[0], split[1], split[2], split[3]);
    if (split[0][0] != 'C' || split[0][1] != '\0')
    {
        printf("returning 0 from validate_camera 0\n");
		return (free_split(split));
    }
    if (split[1] == NULL || split[2] == NULL || split[3] == NULL || split[4] != NULL)
    {
        printf("returning 0 from validate_camera 0.5\n");
        return (free_split(split));
    }
    // add unsigned decimal check
    if (xyz_check(split[1]) == 0)
    {
        printf("returning 0 from validate_camera 3\n");
        return (free_split(split));
    }
    // add vectors check
    if (vectors_check(split[2]) == 0)
    {
        printf("returning 0 from validate_camera 4\n");
        return (free_split(split));
    }
    // add FOV check
    if (degree_check(split[3], 0, 180) == 0)
    {
        printf("returning 0 from camera check 5\n");
        return (free_split(split));
    }
    if (setup_camera(split, map) == 0)
    {
        printf("returning 0 from camera check 6\n");
        return (free_split(split));
    }
    free_split(split);
    element_count->camera++;
    printf("returned 1 from camera check\n");
    return (1);
}
