/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxfunctions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:08 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/25 16:11:15 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

uint32_t ft_pixel(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void write_color(t_color col, t_var *var, int x, int y)
{
    int color = ft_pixel(col.r, col.g, col.b, 255);
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

static void	close_hook(void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
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
	mlx_loop_hook(var->mlx, &close_hook, var->mlx);
	printf("mlxinit success\n");
	return (EXIT_SUCCESS);
}

/*
int mlxinit(t_var *var)
{
	var->aspect_ratio = 16.0 / 9.0;
	//var->image_height = WIDTH / var->aspect_ratio;
	var->image_height = HEIGHT;
	var->image_width = WIDTH;
	if (!(var->mlx = mlx_init(WIDTH, var->image_height, "MiniRT", true)))
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

int mlxinit(t_var *var)
{
	//var->cam.aspect_ratio = 16.0 / 9.0;
	int image_width;
	//var->image_height = WIDTH / var->aspect_ratio;
	//var->image_width = WIDTH;
	image_width = WIDTH / var->aspect_ratio;
	if (!(var->mlx = mlx_init(WIDTH, HEIGHT, "MiniRT", true)))
	{
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(var->screenimage = mlx_new_image(var->mlx, WIDTH, HEIGHT)))
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
}*/
