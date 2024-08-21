/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_light.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:51:00 by tvalimak          #+#    #+#             */
/*   Updated: 2024/08/20 13:50:38 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

/*∗ identifier: L
∗ x,y,z coordinates of the light point: -40.0,50.0,0.0
∗ the light brightness ratio in range [0.0,1.0]: 0.6
∗ (unused in mandatory part)R,G,B colors in range [0-255]: 10, 0, 255*/

// L -40.0,,50.0,0.0 0.6 10,0,255 should give invalid but now
// it doesn't

int validate_light(char *line, t_element_count *element_count, t_map *map)
{
    char **split;

    (void)element_count;
    printf("inside validate light\n");
    split = ft_split(line, ' ');
	if (split[0] == NULL || split[1] == NULL || split[2] == NULL || \
	split[3] == NULL || split[4] != NULL)
    {
        printf("returning 0 from validate light 1\n");
        return (free_split(split));
    }
    printf("%s\n%s\n%s\n%s", split[0], split[1], split[2], split[3]);
    if (split[0][0] != 'L' && split[0][1] != '\0')
    {
        printf("returning 0 from validate light 0\n");
        return (free_split(split));
    }
    if (xyz_check(split[1]) == 0)
    {
        printf("returning 0 from validate_light 2\n");
        return (free_split(split));
    }
    if (pos_decimal_check(split[2]) == 0)
    {
        printf("returning 0 from validate_light 3\n");
        return (free_split(split));
    }
    if (rgb_check(split[3], 0, 255) == 0)
    {
        printf("returning 0 from validate_light 4\n");
        return (free_split(split));
    }
	if (setup_light(split, map) == 0)
        return (free_split(split));
    free_split(split);
    printf("returned 1 from light check\n");
    element_count->light++;
    return (1);
}