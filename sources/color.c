/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:46 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/16 09:32:02 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color color_add(t_color c1, t_color c2)
{
	t_color result;

	result.r = c1.r + c2.r;
	result.b = c1.b + c2.b;
	result.g = c1.g + c2.g;
	return (result);
}

// Multiplies each component of a color (vector) by a scalar
t_color color_multiply_scalar(t_color c, float s)
{
	t_color result;

	result.r = c.r * s;
	result.b = c.b * s;
	result.g = c.g * s;
	return (result);
}

t_color t_color_create(float x, float y, float z)
{
	t_color result;

	result.r = x;
	result.b = y;
	result.g = z;
	return (result);
}
