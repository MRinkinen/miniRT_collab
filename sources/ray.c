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
t_ray ray(t_tuple origin, t_tuple direction)
{
    t_ray ray;

    ray.origin = origin;
    ray.direction = direction;
    return (ray);
}

t_ray generate_ray_for_pixel(t_var *var, int x, int y)
{
    float u;
    float v;
    t_tuple ray_direction;

    // Normalize pixel coordinates to the range [0, 1]
    u = (double)x / (double)(WIDTH - 1);
    v = (double)y / (double)(HEIGHT - 1);

    // Compute the direction of the ray by starting from the lower left corner,
    // and moving horizontally and vertically based on the pixel position
    t_tuple pixel_point = tuple_add(
        var->cam.v_up_left_c,
        tuple_add(
            tuple_multiply(var->cam.view_u, u),
            tuple_multiply(var->cam.view_v, v)
        )
    );

    // Calculate the direction by subtracting the camera's position
    // and normalizing the result to ensure the direction vector has unit length
    ray_direction = normalize(tuple_subtract(pixel_point, var->cam.position));

    // Return the ray with the origin at the camera position, and the computed direction
    return (ray(var->cam.position, ray_direction));
}

/*
t_ray generate_ray_for_pixel(t_var *var, int x, int y)
{
    float u;
    float v;
    t_tuple ray_direction;

    // Normalize pixel coordinates to the range [0, 1]
    u = (float)x / (float)(WIDTH - 1);
    v = (float)y / (float)(var->image_height - 1);

    // Compute the direction of the ray by starting from the lower left corner,
    // and moving horizontally and vertically based on the pixel position
    t_tuple pixel_point = tuple_add(
        var->cam.v_up_left_c,
        tuple_add(
            tuple_multiply(var->cam.view_u, u),
            tuple_multiply(var->cam.view_v, v)
        )
    );

    // Calculate the direction by subtracting the camera's position
    // and normalizing the result to ensure the direction vector has unit length
    ray_direction = normalize(tuple_subtract(pixel_point, var->cam.position));

    // Return the ray with the origin at the camera position, and the computed direction
    return (ray(var->cam.position, ray_direction));
}*/

/*
// Function to generate a ray for a given pixel
t_ray generate_ray_for_pixel(t_var *var, int x, int y)
{
    float u;
    float v;
    t_tuple ray_direction;

    // Calculate normalized coordinates for the current pixel
    u = (float)x / (float)(SCREEN_WIDTH - 1);
    v = (float)y / (float)(var->image_height - 1);
    // Compute the ray direction by shooting through the image plane
    ray_direction = normalize(tuple_subtract(
        tuple_add(var->cam.lower_left_corner,
                  tuple_add(tuple_multiply(var->cam.horizontal, u),
                            tuple_multiply(var->cam.vertical, v))),
        var->cam.position));
    // Return the ray originating from the camera's position, heading towards the computed direction
    return (ray(var->cam.position, ray_direction));
}*/

/*
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
}*/
