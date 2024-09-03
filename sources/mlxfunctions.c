/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxfunctions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:08 by mrinkine          #+#    #+#             */
/*   Updated: 2024/09/03 21:36:18 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void ft_hook(void *param)
{
	t_var *var;

	var = param;

	if (mlx_is_key_down(var->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(var->mlx);
	// if (mlx_is_key_down(var->mlx, MLX_KEY_W) && mlx_is_key_down(var->mlx, MLX_KEY_LEFT_SHIFT))
	// 	var->camreraz -= 1;
	// else if (mlx_is_key_down(var->mlx, MLX_KEY_W))
	// 	var->camrerax += 1;
	// if (mlx_is_key_down(var->mlx, MLX_KEY_S) && mlx_is_key_down(var->mlx, MLX_KEY_LEFT_SHIFT))
	// 	var->camreraz += 1;
	// else if (mlx_is_key_down(var->mlx, MLX_KEY_S))
	// 	var->camrerax -= 1;
	// if (mlx_is_key_down(var->mlx, MLX_KEY_A))
	// 	var->camreray -= 1;
	// if (mlx_is_key_down(var->mlx, MLX_KEY_D))
	// 	var->camreray += 1;
}

void hooks(t_var *var)
{
	// mlx_loop_hook(var->mlx, printimage, var); // If here. Can move camera while program is running
	mlx_loop_hook(var->mlx, ft_hook, var);
}

int mlxinit(t_var *var, t_map *map)
{
	var->cam.position = t_vec3_create(map->camera->x,map->camera->y,map->camera->z);
	var->cam.aspect_ratio = 16.0 / 9.0;
	// Calculate viewport height based on the FOV
	var->fov = map->camera->fov;									// FOV in degrees
	var->theta = var->fov * PI / 180.0;								// Convert FOV to radians
	var->viewport_height = 2.0 * tan(var->theta / 2.0);				// Height of the viewport at a focal length of 1 unit
	var->viewport_width = var->viewport_height * var->cam.aspect_ratio; // Calculate the viewport width based on the aspect ratio
	// Calculate image height and width based on the viewport
	var->image_height = SCREEN_WIDTH / var->cam.aspect_ratio;
	//var->image_height = 600.0;
	var->image_width = 1500.0;
	if (!(var->mlx = mlx_init(SCREEN_WIDTH, var->image_height, "MiniRT", true)))
	{
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(var->testimage = mlx_new_image(var->mlx, var->image_width, var->image_height)))
	{
		mlx_close_window(var->mlx);
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (mlx_image_to_window(var->mlx, var->testimage, 0, 0) == -1)
	{
		mlx_close_window(var->mlx);
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
