/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxfunctions_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:08 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/29 10:08:31 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"

void	move_camera_add(t_var *var, t_tuple move)
{
	print_black(var);
	var->cam.position = tuple_add(var->cam.position, move);
	printimage_low(var, count_resolution_scale(var));
}

void	move_camera_subtract(t_var *var, t_tuple move)
{
	print_black(var);
	var->cam.position = tuple_subtract(var->cam.position, move);
	printimage_low(var, count_resolution_scale(var));
}

void	ft_hook(void *param)
{
	t_var	*var;

	var = param;
	if (mlx_is_key_down(var->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(var->mlx);
	if (mlx_is_key_down(var->mlx, MLX_KEY_W))
		move_camera_add(var, var->cam.forward);
	if (mlx_is_key_down(var->mlx, MLX_KEY_S))
		move_camera_subtract(var, var->cam.forward);
	if (mlx_is_key_down(var->mlx, MLX_KEY_D))
		move_camera_subtract(var, var->cam.right);
	if (mlx_is_key_down(var->mlx, MLX_KEY_A))
		move_camera_add(var, var->cam.right);
	if (mlx_is_key_down(var->mlx, MLX_KEY_E))
		move_camera_add(var, var->cam.up);
	if (mlx_is_key_down(var->mlx, MLX_KEY_Q))
		move_camera_subtract(var, var->cam.up);
	if (mlx_is_key_down(var->mlx, MLX_KEY_SPACE))
		printimage(var, 1);
}

void	hooks(t_var *var)
{
	mlx_loop_hook(var->mlx, ft_hook, var);
}

int	mlxinit(t_var *var)
{
	var->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!var->mlx)
	{
		printf("mlx_init failed\n");
		return (EXIT_FAILURE);
	}
	var->screenimage = mlx_new_image(var->mlx, WIDTH, HEIGHT);
	if (!var->screenimage)
	{
		printf("mlx_new_image failed\n");
		return (EXIT_FAILURE);
	}
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	if (mlx_image_to_window(var->mlx, var->screenimage, 0, 0) == -1)
	{
		printf("mlx_image_to_window failed\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
