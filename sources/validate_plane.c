/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_plane.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:07:31 by tvalimak          #+#    #+#             */
/*   Updated: 2024/08/20 13:50:43 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

/*∗ identifier: pl
∗ x,y,z coordinates of a point in the plane: 0.0,0.0,-10.0
∗ 3d normalized normal vector. In range [-1,1] for each x,y,z axis: 0.0,1.0,0.0
∗ R,G,B colors in range [0-255]: 0,0,225*/

int validate_plane(char *line, t_element_count *element_count, \
    t_map *map)
{
    char **split;

    printf("inside validate plane\n");
    split = ft_split(line, ' ');
    if (!split[1] || !split[2] || !split[3] || split[4])
    {
        printf("returning 0 from validate_plane 0\n");
        return (free_split(split));
    }
    printf("%s\n%s\n%s\n%s", split[0], split[1], split[2], split[3]);
    if (split[0][2] != '\0')
    {
        printf("returning 0 from validate_plane 1\n");
        return (free_split(split));
    }
    if (xyz_check(split[1]) == 0)
    {
        printf("returning 0 from validate_plane 2\n");
        return (free_split(split));
    }
    if (vectors_check(split[2]) == 0)
    {
        printf("returning 0 from validate_plane 3\n");
        return (free_split(split));
    }
    if (rgb_check(split[3], 0, 255) == 0)
    {
        printf("returning 0 from validate_plane 4\n");
        return (free_split(split));
    }
    if (setup_plane(split, map) == 0)
        return (free_split(split));
    free_split(split);
    printf("returned 1 from validate_plane\n");
    element_count->plane++;
    return (1);
}
