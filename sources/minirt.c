/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/20 10:48:06 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color ray_color(const t_ray *r, hittable_list *world, t_vec3 camera_pos)
{
	t_hit rec;
	t_color background;

	if (hittable_list_hit(world, r, 0.001f, INFINITY, &rec))
	{
		//  Calculate the distance from the camera to the intersection point
		float distance = calculate_distance(camera_pos, rec.p);

		// Define attenuation factors (tuned for your scene)
		float constant = 1.0;
		float linear = 0.09;
		float quadratic = 0.032;

		// Calculate the attenuation based on the distance
		float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));
		t_color final_color = color_multiply_scalar(rec.color, attenuation);
		return final_color;
	}

	// Background color if no hit occurs
	background = t_color_create(0.1f, 0.1f, 0.1f);
	return background;
}

void intitscreen(t_var *var)
{
	float focal_length = 1.0;
	float viewport_height = 1.0; // Jos ei 1, niin pallukat soikeita
	float viewport_width = viewport_height * ((float)SCREEN_WIDHT / var->image_height);
	var->camera_center = t_vec3_create(var->camrerax, var->camreray, var->camreraz);

	t_vec3 viewport_u = t_vec3_create(viewport_width, 0, 0);
	t_vec3 viewport_v = t_vec3_create(0, -viewport_height, 0);

	var->pixel_delta_u = t_vec3_divide_scalar(&viewport_u, SCREEN_WIDHT);
	var->pixel_delta_v = t_vec3_divide_scalar(&viewport_v, var->image_height);

	t_vec3 temp1 = t_vec3_create(0, 0, -focal_length);
	t_vec3 temp2 = t_vec3_divide_scalar(&viewport_u, 2);
	t_vec3 temp3 = t_vec3_divide_scalar(&viewport_v, 2);
	t_vec3 temp4 = t_vec3_add_vectors(&temp1, &temp2);
	t_vec3 temp5 = t_vec3_add_vectors(&temp4, &temp3);
	t_vec3 viewport_upper_left = t_vec3_subtract_vectors(&var->camera_center, &temp5);

	t_vec3 temp = t_vec3_add_vectors(&var->pixel_delta_u, &var->pixel_delta_v);
	t_vec3 temp_result = t_vec3_multiply_scalar(&temp, 0.5);
	var->pixel00_loc = t_vec3_add_vectors(&viewport_upper_left, &temp_result);
}

void screenloop(t_var *var, hittable_list world)
{
	t_vec3 temp_u;
	t_vec3 temp_v;
	t_vec3 pixel_center;
	t_vec3 ray_direction;
	t_ray ray;
	t_color pixel_color;

	for (int j = 0; j < var->image_height; j++)
	{
		for (int i = 0; i < SCREEN_WIDHT; i++)
		{
			temp_u = t_vec3_multiply_scalar(&var->pixel_delta_u, i);
			temp_v = t_vec3_multiply_scalar(&var->pixel_delta_v, j);
			pixel_center = t_vec3_add_vectors(&var->pixel00_loc, &temp_u);
			pixel_center = t_vec3_add_vectors(&pixel_center, &temp_v);
			ray_direction = t_vec3_subtract_vectors(&pixel_center, &var->camera_center);
			ray = ray_create(&var->camera_center, &ray_direction);
			pixel_color = ray_color(&ray, &world, var->camera_center);
			write_color(pixel_color, var, j, i);
		}
	}
}

void printimage(void *param)
{
	t_var *var;
	var = param;

	hittable_list world;

	hittable_list_init(&world);

	t_sphere s1 = sphere_create(t_vec3_create(0, 25, 200), 100, t_color_create(60, 50, 240));
	t_sphere s2 = sphere_create(t_vec3_create(-27, 0, 100), 15, t_color_create(200, 56, 100));
	t_sphere s3 = sphere_create(t_vec3_create(-5, 0, 100), 20, t_color_create(200, 200, 200));
	t_sphere s4 = sphere_create(t_vec3_create(27, -0, 100), 25, t_color_create(200, 50, 200));

	t_sphere s5 = sphere_create(t_vec3_create(-28, -5, 85), 3, t_color_create(10, 10, 10));
	t_sphere s6 = sphere_create(t_vec3_create(-28, 5, 85), 3, t_color_create(10, 10, 10));

	hittable_list_add(&world, (t_hittable *)&s1);
	hittable_list_add(&world, (t_hittable *)&s2);
	hittable_list_add(&world, (t_hittable *)&s3);
	hittable_list_add(&world, (t_hittable *)&s4);
	hittable_list_add(&world, (t_hittable *)&s5);
	hittable_list_add(&world, (t_hittable *)&s6);

	intitscreen(var);
	screenloop(var, world);
}

int main()
{
	t_var var;

	if (mlxinit(&var))
		return (EXIT_FAILURE);
	printimage(&var);
	hooks(&var);

	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	return (EXIT_SUCCESS);
}
