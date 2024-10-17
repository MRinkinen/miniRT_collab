/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:02 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/17 17:13:59 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// Function to create a new ray given an origin and a direction
t_ray ray(t_tuple origin, t_tuple direction)
{
    t_ray ray;

    ray.origin = origin;
    ray.direction = direction;
    return (ray);
}

// Function to generate a ray for a given pixel
t_ray generate_ray_for_pixel(t_var *var, int x, int y)
{
    float u;
    float v;
    t_tuple ray_direction;

    u = (float)x / (float)(SCREEN_WIDTH - 1);
    v = (float)y / (float)(var->image_height - 1);
    ray_direction = normalize(tuple_subtract(
        tuple_add(var->cam.lower_left_corner,
                  tuple_add(tuple_multiply(var->cam.horizontal, u),
                            tuple_multiply(var->cam.vertical, v))),
        var->cam.position));

    return (ray(var->cam.position, ray_direction));
}
