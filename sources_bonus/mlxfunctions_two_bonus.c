/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxfunctions_two_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:47:03 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/29 10:11:39 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"

uint32_t	ft_pixel(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	write_color(t_color col, t_var *var, int x, int y)
{
	int	color;

	color = ft_pixel(col.r, col.g, col.b, 255);
	mlx_put_pixel(var->screenimage, x, y, color);
}

void	print_black(t_var *var)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			write_color(t_color_create(0, 0, 0), var, x, y);
			x++;
		}
		y++;
	}
}

int	count_resolution_scale(t_var *var)
{
	int	resolution_scale;

	if (var->num_objects < 4)
		resolution_scale = 2;
	else if (var->num_objects < 10)
		resolution_scale = 3;
	else if (var->num_objects < 50)
		resolution_scale = 4;
	else if (var->num_objects < 100)
		resolution_scale = 8;
	else
		resolution_scale = 16;
	return (resolution_scale);
}
