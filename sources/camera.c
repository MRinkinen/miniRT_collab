/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:29:08 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 16:29:21 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	set_world_up(t_var *var, t_tuple *world_up, t_tuple forward)
{
	*world_up = vector(0, 1, 0);
	if (fabs(forward.y) >= 0.9999)
	{
		if (forward.y < 0)
			*world_up = vector(0, 0, 1);
		else
			*world_up = vector(0, 0, -1);
	}
	var->cam.right = normalize(cross(var->cam.forward, *world_up));
}

int	initialize_camera(t_var *var, t_cam *camera, t_map *map)
{
	t_tuple	world_up;

	var->cam.position = point(map->camera->x, map->camera->y, map->camera->z);
	var->aspect_ratio = (float) WIDTH / HEIGHT;
	var->cam.focal_length = (WIDTH / 2) / \
	tanf((map->camera->fov * (PI / 180)) / 2);
	var->cam.view_w = WIDTH * 2;
	var->cam.viewp_h = var->cam.view_w / var->aspect_ratio;
	var->cam.forward = normalize(vector(map->camera->nx, \
	map->camera->ny, map->camera->nz));
	set_world_up(var, &world_up, var->cam.forward);
	var->cam.up = cross(var->cam.right, var->cam.forward);
	var->cam.view_u = tuple_multiply(var->cam.right, var->cam.view_w);
	var->cam.view_v = tuple_multiply(var->cam.up, var->cam.viewp_h);
	var->cam.delta_u = tuple_divide(var->cam.view_u, WIDTH);
	var->cam.delta_v = tuple_divide(var->cam.view_v, HEIGHT);
	var->cam.focal = tuple_multiply(var->cam.forward, var->cam.focal_length);
	var->cam.v_up_left_c = tuple_subtract(var->cam.position, var->cam.focal);
	var->cam.v_up_left_c = tuple_subtract(var->cam.v_up_left_c, \
	tuple_divide(var->cam.view_u, 2));
	var->cam.v_up_left_c = tuple_subtract(var->cam.v_up_left_c, \
	tuple_divide(var->cam.view_v, 2));
	var->cam.loc_00 = tuple_add(var->cam.v_up_left_c, \
	tuple_multiply(tuple_add(var->cam.delta_u, var->cam.delta_v), 0.5));
	return (EXIT_SUCCESS);
}
