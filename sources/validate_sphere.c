/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_sphere.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:59:22 by tvalimak          #+#    #+#             */
/*   Updated: 2024/08/20 16:32:37 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

/*∗ identifier: sp
∗ x,y,z coordinates of the sphere center: 0.0,0.0,20.6
∗ the sphere diameter: 12.6
∗ R,G,B colors in range [0-255]: 10, 0, 255*/

int	validate_sphere(char *line, t_element_count *element_count, t_map *map)
{
    char **split;

    printf("inside validate sphere\n");
    split = ft_split(line, ' ');
    if (!split[1] || !split[2] || !split[3] || split[4])
    {
        printf("returning 0 from validate_sphere 0\n");
        return (free_split(split));
    }
    printf("%s\n%s\n%s\n%s", split[0], split[1], split[2], split[3]);
    if (split[0][2] != '\0')
    {
        printf("returning 0 from validate_sphere 1\n");
        return (free_split(split));
    }
    if (xyz_check(split[1]) == 0)
    {
        printf("returning 0 from validate_sphere 2\n");
        return (free_split(split));
    }
    if (decimal_check(split[2], 0, 100) == 0)
    {
        printf("returning 0 from validate_sphere 3\n");
        return (free_split(split));
    }
    if (rgb_check(split[3], 0, 255) == 0)
    {
        printf("returning 0 from validate_sphere 4\n");
        return (free_split(split));
    }
    if (setup_spheres(split, map) == 0)
        return (free_split(split));
    free_split(split);
    printf("returned 1 from validate_sphere\n");
    element_count->sphere++;
    return (1);
}
