/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:02 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/15 13:34:06 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// Function to create a new ray given an origin and a direction
t_ray ray(t_tuple origin, t_tuple direction)
{
    t_ray new_ray;
    new_ray.origin = origin;
    new_ray.direction = direction;
    return new_ray;
}

// Function to generate a ray for a given pixel
t_ray generate_ray_for_pixel(int x, int y, int image_width, int image_height, t_cam *cam)
{
    float u = (float)x / (float)(image_width - 1);
    float v = (float)y / (float)(image_height - 1);

    t_tuple ray_direction = normalize(tuple_subtract(
        tuple_add(cam->lower_left_corner,
                  tuple_add(tuple_multiply(cam->horizontal, u),
                            tuple_multiply(cam->vertical, v))),
        cam->position));

    return ray(cam->position, ray_direction);
}
