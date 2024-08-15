/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:02 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/15 12:02:04 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_ray ray_create(const t_vec3 *origin, const t_vec3 *direction)
{
    t_ray ray;

    ray.orig = *origin;
    ray.dir = *direction;
    return (ray);
}

t_vec3 ray_origin(const t_ray *ray)
{
    return (ray->orig);
}

t_vec3 ray_direction(const t_ray *ray)
{
    return (ray->dir);
}

t_vec3 ray_at(const t_ray *ray, float t)
{
    t_vec3 scaled_dir;

    scaled_dir = t_vec3_multiply_scalar(&ray->dir, t);
    return t_vec3_add_vectors(&ray->orig, &scaled_dir);
}
