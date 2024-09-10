/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:46 by mrinkine          #+#    #+#             */
/*   Updated: 2024/09/10 14:05:07 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color color_add(t_color c1, t_color c2)
{
	t_color result;

	result.r = c1.r + c2.r;
	result.g = c1.g + c2.g;
	result.b = c1.b + c2.b;
	return (result);
}

// Multiplies each component of a color (vector) by a scalar
t_color color_multiply_scalar(t_color c, float s)
{
	t_color result;

	result.r = c.r * s;
	result.g = c.g * s;
	result.b = c.b * s;
	return (result);
}

t_color t_color_create(float r, float g, float b)
{
	t_color result;

	// result.r = fmax(0.0, fmin(1.0, r));
    // result.g = fmax(0.0, fmin(1.0, g));
    // result.b = fmax(0.0, fmin(1.0, b));

    result.r =  r;
    result.g =  g;
    result.b = 	b;
	return (result);
}

t_color hadamard_product(t_color c1, t_color c2)
{
	t_color result;

	result.r = c1.r * c2.r;
	result.g = c1.g * c2.g;
	result.b = c1.b * c2.b;
	return (result);
}

t_color clamp_and_normalize_color(t_color color)
{
	// Clamp the color values to the range [0.0, 1.0]
	color.r = fmin(fmax(color.r, 0.0f), 1.0f);
	color.g = fmin(fmax(color.g, 0.0f), 1.0f);
	color.b = fmin(fmax(color.b, 0.0f), 1.0f);

	// Calculate the magnitude of the color vector
	float magnitude = sqrt(color.r * color.r + color.g * color.g + color.b * color.b);

	// Normalize the color vector if its magnitude is greater than 1.0
	if (magnitude > 1.0f)
	{
		color.r /= magnitude;
		color.g /= magnitude;
		color.b /= magnitude;
	}
	return color;
}
