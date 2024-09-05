/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:02 by mrinkine          #+#    #+#             */
/*   Updated: 2024/09/05 19:07:36 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// t_ray ray_create(const t_vec3 *origin, const t_vec3 *direction)
// {
//     t_ray ray;

//     ray.origin = *origin;
//     ray.direction = *direction;
//     return (ray);
// }

// t_vec3 ray_origin(const t_ray *ray)
// {
//     return (ray->origin);
// }

// t_vec3 ray_direction(const t_ray *ray)
// {
//     return (ray->direction);
// }

// t_vec3 ray_at(const t_ray *ray, float t)
// {
//     t_vec3 scaled_dir;

//     scaled_dir = t_vec3_multiply_scalar(&ray->direction, t);
//     return t_vec3_add_vectors(&ray->origin, &scaled_dir);
// }

// t_ray generate_ray(t_cam *camera, float u, float v)
// {
//     float theta = camera->fov * M_PI / 180.0f;
//     float half_height = tan(theta / 2);

//     // t_vec3 lower_left_corner = subtract_vectors(
//     //     subtract_vectors(
//     //         subtract_vectors(camera->position,
//     //                          multiply_vector_scalar(camera->right, half_width)),
//     //         multiply_vector_scalar(camera->up, half_height)),
//     //     camera->forward);

//     // t_vec3lefttemp3 = t_vec3_multiply_scalar(camera->right, half_width)), t_vec3_multiply_scalar(camera->up, half_height)), &camera->forward);
//     // float half_width = camera->aspect_ratio * half_height;
//     // t_vec3 lefttemp4 = t_vec3_subtract_vectors(camera->position,
//     // t_vec3 lefttemp5 = t_vec3_subtract_vectors(lefttemp4

//     // t_vec3 lower_left_corner = t_vec3_subtract_vectors(
//     t_vec3 horizontal = t_vec3_multiply_scalar(&camera->right, 2 * half_width);
//     t_vec3 vertical = t_vec3_multiply_scalar(&camera->up, 2 * half_height);
//     t_vec3 temp = t_vec3_multiply_scalar(&horizontal, u);
//     t_vec3 temp1 = t_vec3_add_vectors(&lower_left_corner, &temp);
//     t_vec3 temp2 = t_vec3_multiply_scalar(&vertical, v);
//     t_vec3 direction = t_vec3_add_vectors(&temp1, &temp2);
//     direction = subtract_vectors(direction, camera->position);

//     t_ray ray;
//     ray.orig = camera->position;
//     ray.dir = normalize_vector(direction);
//     return ray;
// }
