/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxfunctions_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:47:03 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 12:16:23 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

uint32_t	ft_pixel(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}
/*
void	write_color(t_color col, t_var *var, int x, int y)
{
	int	color;

	color = ft_pixel(col.r, col.g, col.b, 255);
	mlx_put_pixel(var->screenimage, x, y, color);
}*/
