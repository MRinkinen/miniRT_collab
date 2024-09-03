/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/09/03 21:37:55 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"
#include "../includes/test_functions.h"

void print_clock_face(t_var *var) 
{
    float CLOCK_RADIUS = var->image_width * 1.0 / 8.0;
    float CENTER_X = var->image_width / (float)2.0;
    float CENTER_Y = var->image_height / (float)2.0;
    const int MARK_SIZE = 10; // Size of the mark, 10 pixels

    Tuple twelve_oclock = point(0, 0, 1); // 12 o'clock at the top center
    int hours = 12;
    float angle_per_hour = (float)3.1415 / 6; // 2 * M_PI / 12

	printf("Center: (%f, %f)\n", CENTER_X, CENTER_Y);
	printf("Camera fov: %f\n", var->fov);

    // Draw each hour mark
    for (int hour = 0; hour < hours; hour++) {
        t_matrix *rotation = rotation_y(angle_per_hour * hour);
        Tuple hour_pos = apply_transformation(rotation, &twelve_oclock);

    // Scale and translate position
    float px = CENTER_X + (float)(hour_pos.x * CLOCK_RADIUS);
    float py = CENTER_Y - (float)(hour_pos.z * CLOCK_RADIUS);  // Adjust y-axis if necessary

    printf("Hour %d: px = %f, py = %f\n", hour, px, py);

        // Draw a small square around the hour mark
        for (int dx = -MARK_SIZE / 2; dx <= MARK_SIZE / 2; dx++) {
            for (int dy = -MARK_SIZE / 2; dy <= MARK_SIZE / 2; dy++) {
                int final_px = px + dx;
                int final_py = py + dy;
                if (final_px >= 0 && final_px < var->image_width && final_py >= 0 && final_py < var->image_height) {
                    write_color(t_color_create(0, 0, 0), var, final_px, final_py);  // Draw black mark
                }
            }
        }
        free(rotation); // Free the matrix after using it
    }
}

void printimage(void *param, t_map *map)
{
	t_var *var;
 	var = param;
	(void)map;
	for (int j = 0; j < (int)var->image_height; j++)
	{
		for (int i = 0; i < SCREEN_WIDTH; i++)
		{
			write_color(t_color_create(1,1,1), var, i, j);
		}
	}
	print_clock_face(var);
}

int main(int argc, char **argv)
{
	t_var var;
	t_element_count element_count;
	t_map *map;

	if (argc != 2)
	{
		ft_printf("Error in arguments, just give a map filename\n");
		return (0);
	}
	ft_memset(&element_count, 0, sizeof(t_element_count));
	map = malloc(sizeof(t_map));
	setup_data(&element_count, map);
	if (!map)
		return (0);
	if (read_to_parse(&element_count, map, argv) == 0)
		return (0);
	print_data(map);
	printf("image width: %f\n", var.image_width);
	printf("image height: %f\n", var.image_height);
	if (mlxinit(&var, map) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	printimage(&var, map);
	hooks(&var);
	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	terminate_data(map, "program ended successfully\n");
	return (EXIT_SUCCESS);
}
