/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:46 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/15 13:09:51 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// t_color color_add(t_color c1, t_color c2)
// {
// 	t_color result;

// 	result.r = c1.r + c2.r;
// 	result.g = c1.g + c2.g;
// 	result.b = c1.b + c2.b;
// 	return (result);
// }

// // Clamping function
// int clamp(int value, int min, int max)
// {
//     if (value < min) return min;
//     if (value > max) return max;
//     return value;
// }

// // Function to create a color with clamped values
// t_color t_color_create(float r, float g, float b)
// {
//     t_color color;

//     color.r = clamp(r, 0, 255);
//     color.g = clamp(g, 0, 255);
//     color.b = clamp(b, 0, 255);
//     return color;
// }

// // t_color t_color_create(float r, float g, float b)
// // {
// // 	t_color result;

// // 	// result.r = fmax(0.0, fmin(1.0, r));
// //     // result.g = fmax(0.0, fmin(1.0, g));
// //     // result.b = fmax(0.0, fmin(1.0, b));

// //     result.r =  r;
// //     result.g =  g;
// //     result.b = 	b;
// // 	return (result);
// // }

// // // Function to add two colors
// // t_color add_colors(t_color c1, t_color c2)
// // {
// //     return (t_color){c1.r + c2.r, c1.g + c2.g, c1.b + c2.b};
// // }

// // Function to subtract two colors
// t_color subtract_colors(t_color c1, t_color c2)
// {
//     return (t_color){c1.r - c2.r, c1.g - c2.g, c1.b - c2.b};
// }

// // Function to multiply a color by a scalar
// t_color multiply_color_scalar(t_color c, double scalar)
// {
//     return (t_color){c.r * scalar, c.g * scalar, c.b * scalar};
// }

// // Function to multiply two colors (Hadamard product)
// t_color multiply_colors(t_color c1, t_color c2)
// {
//     return (t_color){c1.r * c2.r, c1.g * c2.g, c1.b * c2.b};
// }

// Clamping function
int clamp(int value, int min, int max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Function to create a color with clamped values
t_color t_color_create(int r, int g, int b)
{
    t_color color;
    color.r = clamp(r, 0, 255);
    color.g = clamp(g, 0, 255);
    color.b = clamp(b, 0, 255);
    return color;
}

// Function to add two colors with clamping
t_color color_add(t_color c1, t_color c2)
{
    t_color result;
    result.r = clamp(c1.r + c2.r, 0, 255);
    result.g = clamp(c1.g + c2.g, 0, 255);
    result.b = clamp(c1.b + c2.b, 0, 255);
    return result;
}

// Function to multiply a color by a scalar with clamping
t_color multiply_color_scalar(t_color color, float scalar)
{
    t_color result;
    result.r = clamp((int)(color.r * scalar), 0, 255);
    result.g = clamp((int)(color.g * scalar), 0, 255);
    result.b = clamp((int)(color.b * scalar), 0, 255);
    return result;
}

// Function to multiply two colors with clamping
t_color multiply_colors(t_color a, t_color b)
{
    t_color result;
    result.r = clamp((a.r * b.r) / 255, 0, 255);
    result.g = clamp((a.g * b.g) / 255, 0, 255);
    result.b = clamp((a.b * b.b) / 255, 0, 255);
    return result;
}


