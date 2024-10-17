/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxfunctions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:08 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/17 16:12:58 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void write_color(t_color col, t_var *var, int x, int y)
{
    int color = ft_pixel(col.r, col.b, col.g, 255);
    mlx_put_pixel(var->screenimage, x, y, color);
}

void ft_hook(void *param)
{
	t_var *var;

	var = param;

	if (mlx_is_key_down(var->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(var->mlx); // Tarviiko tahan myos free_scene?
}

void hooks(t_var *var)
{
	mlx_loop_hook(var->mlx, ft_hook, var);
}

int mlxinit(t_var *var)
{
	var->cam.aspect_ratio = 16.0 / 9.0;
	var->image_height = SCREEN_WIDTH / var->cam.aspect_ratio;
	var->image_width = SCREEN_WIDTH;
	if (!(var->mlx = mlx_init(SCREEN_WIDTH, var->image_height, "MiniRT", true)))
	{
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(var->screenimage = mlx_new_image(var->mlx, var->image_width, var->image_height)))
	{
		mlx_close_window(var->mlx);
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (mlx_image_to_window(var->mlx, var->screenimage, 0, 0) == -1)
	{
		mlx_close_window(var->mlx);
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
