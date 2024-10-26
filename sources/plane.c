/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:43:32 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 02:45:11 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

void    create_planes(t_var *var, t_map *map, int *obj_index)
{
    t_planes    *current_plane;
    t_tuple     orientation;
    t_tuple     center;
    t_tuple     normal;
    t_color     color;

    current_plane = map->planes;
    while (current_plane != NULL)
    {
        orientation = normalize(vector(current_plane->nx, current_plane->ny, current_plane->nz));
        center = point(current_plane->x, current_plane->y, current_plane->z);
        normal = vector(current_plane->nx, current_plane->ny, current_plane->nz);
        color = t_color_create(current_plane->r, current_plane->g, current_plane->b);
        var->objects[*obj_index].type = PLANE;
        var->objects[*obj_index].data.plane.center;
        var->objects[*obj_index].data.plane.point = center;
        var->objects[*obj_index].data.plane.normal = normal;
        var->objects[*obj_index].data.plane.color = color;
		var->objects[*obj_index].data.plane.orientation = orientation;
        current_plane = current_plane->next;
        (*obj_index)++;
    }
}

t_tuple vector_subtract(t_tuple a, t_tuple b)
{
    return ((t_tuple){
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	});
}

int intersect_plane(t_ray *ray, t_plane *plane, float *t)
{
	float		denominator;
	float		numerator;
	t_tuple		oc;

	denominator = dot(ray->direction, plane->normal);
	if (fabs(denominator) < EPSILON)
		return (0);
	oc = vector_subtract(plane->point, ray->origin);
	numerator = dot(oc, plane->orientation);
	*t = numerator / denominator;
	return (*t > 0.0);
}
