/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxfunctions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:08 by mrinkine          #+#    #+#             */
/*   Updated: 2024/09/07 15:10:37 by mrinkine         ###   ########.fr       */
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
	(void)map;
	var->cam.aspect_ratio = 16.0 / 9.0;
	var->image_height = SCREEN_WIDTH / var->cam.aspect_ratio;
	var->image_width = SCREEN_WIDTH;
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
