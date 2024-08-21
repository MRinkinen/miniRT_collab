/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxfunctions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:08 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/21 18:56:47 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void ft_hook(void *param)
{
	t_var *var;

	var = param;

	if (mlx_is_key_down(var->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(var->mlx);
	if (mlx_is_key_down(var->mlx, MLX_KEY_W) && mlx_is_key_down(var->mlx, MLX_KEY_LEFT_SHIFT))
		var->camreraz -= 1;
	else if (mlx_is_key_down(var->mlx, MLX_KEY_W))
		var->camrerax += 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_S) && mlx_is_key_down(var->mlx, MLX_KEY_LEFT_SHIFT))
		var->camreraz += 1;
	else if (mlx_is_key_down(var->mlx, MLX_KEY_S))
		var->camrerax -= 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_A))
		var->camreray -= 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_D))
		var->camreray += 1;
}

void hooks(t_var *var)
{
	// mlx_loop_hook(var->mlx, printimage, var); // If here. Can move camera while program is running
	mlx_loop_hook(var->mlx, ft_hook, var);
}
t_ambient_light setambientlight(t_map *map)
{
	t_ambient_light temp;

	temp.color = t_color_create(map->ambient->r, map->ambient->b, map->ambient->g);
	temp.intensity = map->ambient->ratio;
	return (temp);
}

int mlxinit(t_var *var, t_map *map)
{
	var->camrerax = map->camera->x;
	var->camreray = map->camera->y;
	var->camreraz = map->camera->z;
	var->alight = setambientlight(map);
	// var->aspect_ratio = 16.0 / 9.0;
	// var->image_height = (float)SCREEN_WIDHT / var->aspect_ratio;

	// Define aspect ratio (e.g., 16:9)
	var->aspect_ratio = 16.0 / 9.0;

	// Calculate viewport height based on the FOV
	var->fov = map->camera->fov;									// FOV in degrees
	var->theta = var->fov * PI / 180.0;								// Convert FOV to radians
	var->viewport_height = 2.0 * tan(var->theta / 2.0);				// Height of the viewport at a focal length of 1 unit
	var->viewport_width = var->viewport_height * var->aspect_ratio; // Calculate the viewport width based on the aspect ratio

	// Calculate image height and width based on the viewport
	var->image_height = SCREEN_WIDHT / var->aspect_ratio;
	printf("w: %i h: %f\n", SCREEN_WIDHT, var->image_height);
	if (!(var->mlx = mlx_init(SCREEN_WIDHT, var->image_height, "MiniRT", true)))
	{
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(var->testimage = mlx_new_image(var->mlx, SCREEN_WIDHT, var->image_height)))
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
