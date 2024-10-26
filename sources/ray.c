/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:02 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/24 10:48:55 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// Function to create a new ray given an origin and a direction
t_ray   ray(t_tuple origin, t_tuple direction)
{
    t_ray   ray;

    ray.origin = origin;
    ray.direction = direction;
    return (ray);
}

t_ray   generate_ray_for_pixel(t_var *var, int x, int y)
{
    float       u;
    float       v;
    t_tuple     ray_direction;

    u = (double)x / (double)(WIDTH - 1);
    v = (double)y / (double)(HEIGHT - 1);
    t_tuple pixel_point = tuple_add(
        var->cam.v_up_left_c,
        tuple_add(
            tuple_multiply(var->cam.view_u, u),
            tuple_multiply(var->cam.view_v, v)
        )
    );
    ray_direction = normalize(tuple_subtract(pixel_point, var->cam.position));
    return (ray(var->cam.position, ray_direction));
}
