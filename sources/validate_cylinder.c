/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_cylinder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:14:50 by tvalimak          #+#    #+#             */
/*   Updated: 2024/08/20 18:42:26 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

/*identifier: cy
∗ x,y,z coordinates of the center of the cylinder: 50.0,0.0,20.6, xyz check
∗ 3d normalized vector of axis of cylinder. In range [-1,1] for each x,y,z axis:
0.0,0.0,1.0, vectors check
∗ the cylinder diameter: 14.2, decimal check
∗ the cylinder height: 21.42, decimal check
∗ R,G,B colors in range [0,255]: 10, 0, 255, rgb check*/

int validate_cylinder(char *line, t_element_count *element_count, \
    t_map *map)
{
    char **split;

    printf("inside validate cylinder\n");
    split = ft_split(line, ' ');
    if (!split[1] || !split[2] || !split[3] || !split[4] || !split[5] || split[6])
    {
        printf("returning 0 from validate_cylinder 0\n");
        return (free_split(split));
    }
    printf("%s\n%s\n%s\n%s\n%s\n%s\n", split[0], split[1], split[2], split[3], split[4], split[5]);
    if (split[0][2] != '\0')
    {
        printf("returning 0 from validate_cylinder 1\n");
        return (free_split(split));
    }
    if (xyz_check(split[1]) == 0)
    {
        printf("returning 0 from validate_cylinder 2\n");
        return (free_split(split));
    }
    if (vectors_check(split[2]) == 0)
    {
        printf("returning 0 from validate_cylinder 3\n");
        return (free_split(split));
    }
    if (decimal_check(split[3], 0, 100) == 0)
    {
        printf("returning 0 from validate_cylinder 4\n");
        return (free_split(split));
    }
    if (decimal_check(split[4], 0, 100) == 0)
    {
        printf("returning 0 from validate_cylinder 5\n");
        return (free_split(split));
    }
    if (rgb_check(split[5], 0, 256) == 0)
    {
        printf("returning 0 from validate_cylinder 6\n");
        return (free_split(split));
    }
    if (setup_cylinder(split, map) == 0)
        return (free_split(split));
    free_split(split);
    printf("returned 1 from validate_cylinder\n");
    element_count->cylinder++;
    return (1);
}
