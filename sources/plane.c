/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:43:32 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/25 00:43:33 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

bool intersect_plane(const t_ray *ray, const t_plane *plane, float *t)
{
    float denom = dot(plane->normal, ray->direction);
    if (fabs(denom) > EPSILON)
    {
        t_tuple p0l0 = tuple_subtract(plane->point, ray->origin);
        *t = dot(p0l0, plane->normal) / denom;
        return (*t >= 0);
    }
    return false;
}
