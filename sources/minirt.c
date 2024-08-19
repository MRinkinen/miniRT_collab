/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/19 13:18:54 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// t_color ray_color(const t_ray *r, hittable_list *world, t_vec3 camera_pos)
// {
// 	t_hit rec;
// 	t_color background;

// 	if (hittable_list_hit(world, r, 0, INFINITY, &rec)) // Muuteltu rajusti!!
// 	{
// 		float distance = calculate_distance(camera_pos, r->dir);

// 		// // Define attenuation factors
// 		// float constant = 1.0;
// 		// float linear = 0.09;
// 		// float quadratic = 0.032;

// 		// // Calculate the attenuation based on the distance
// 		// float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
// 		float attenuation = distance;
// 		// Apply the attenuation to the object's color
// 		t_color temp = t_color_create(rec.normal.x, rec.normal.y, rec.normal.z);
// 		t_color final_color = color_multiply_scalar(temp, attenuation);
// 		//  printf("%f\n", temp.e[0]);
// 		//   t_vec3 temp = t_vec3_create(255, 45, 68);
// 		return (final_color);
// 		// return (color_multiply_scalar(temp, t_vec3_magnitude(&r->dir) * 0.50)); // Ei loydy hyvaa lukua!!!
// 	}

// 	background = t_color_create(0.1, 0.1, 0.1);
// 	// t_vec3 unit_direction = t_vec3_unit_vector(&r->dir);
// 	// float t = 0.2 * (unit_direction.y + 1.0);
// 	// t_color white = t_color_create(1.0, 1.0, 1.0);
// 	// t_color blue = t_color_create(0.4, 0.2, 0.2);
// 	// t_color temp_white = color_multiply_scalar(white, 1.0 - t);
// 	// t_color temp_blue = color_multiply_scalar(blue, t);
// 	// return color_add(temp_blue, temp_white);
// 	return (background);
// }

t_color ray_color(const t_ray *r, hittable_list *world, t_vec3 camera_pos)
{
	t_hit rec;
	t_color background;

	if (hittable_list_hit(world, r, 0.001f, INFINITY, &rec))
	{
		// printf("dasfsgdhfjgkhghfds\n");
		//  Calculate the distance from the camera to the intersection point
		float distance = calculate_distance(camera_pos, rec.p);

		// Define attenuation factors (tuned for your scene)
		float constant = 1.0;
		float linear = 0.09;
		float quadratic = 0.032;

		// Calculate the attenuation based on the distance
		float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));

		t_color final_color = color_multiply_scalar(rec.color, attenuation);
		// // Use the normal vector to create a base color
		// t_color base_color = t_color_create(fabs(rec.normal.x), fabs(rec.normal.y), fabs(rec.normal.z));

		// // Apply the attenuation to the base color
		// t_color final_color = color_multiply_scalar(base_color, attenuation);

		return final_color;
	}

	// Background color if no hit occurs
	background = t_color_create(0.1f, 0.1f, 0.1f);
	return background;
}

void initobjects(hittable_list *world)
{

	t_sphere s1 = sphere_create(t_vec3_create(0, 0, -220), 20, t_color_create(100, 100, 100));
	// t_sphere s2 = sphere_create(t_vec3_create(20, -20, -180), 20, t_color_create(100, 100, 100));
	// t_sphere s3 = sphere_create(t_vec3_create(-20, -20, -160), 20, t_color_create(100, 100, 100));
	// t_sphere s4 = sphere_create(t_vec3_create(-20, 20, -100), 20, t_color_create(100, 100, 100));
	//  t_sphere s1b = sphere_create(t_vec3_create(-10, -10, -100), 10);
	//  t_sphere s2b = sphere_create(t_vec3_create(10, 10, -100), 10);
	//  t_sphere s3b = sphere_create(t_vec3_create(-10, 10, -100), 10);
	//  t_sphere s4b = sphere_create(t_vec3_create(10, -10, -100), 10);

	hittable_list_add(world, (t_hittable *)&s1);
	// hittable_list_add(&world, (t_hittable *)&s2);
	// hittable_list_add(&world, (t_hittable *)&s3);
	// hittable_list_add(&world, (t_hittable *)&s4);
	//  hittable_list_add(&world, (t_hittable *)&s1b);
	//  hittable_list_add(&world, (t_hittable *)&s2b);
	//  hittable_list_add(&world, (t_hittable *)&s3b);
	//  hittable_list_add(&world, (t_hittable *)&s4b);
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

			// printf("Ray Direction (Pixel %d, %d): (%f, %f, %f)\n", i, j, ray_direction.x, ray_direction.y, ray_direction.z);

			ray = ray_create(&var->camera_center, &ray_direction);
			pixel_color = ray_color(&ray, &world, var->camera_center);
			write_color(pixel_color, var, j, i);
		}
	}

	// for (int j = 0; j < var->image_height; j++)
	// {
	// 	for (int i = 0; i < SCREEN_WIDHT; i++)
	// 	{
	// 		temp_u = t_vec3_multiply_scalar(&var->pixel_delta_u, i);
	// 		temp_v = t_vec3_multiply_scalar(&var->pixel_delta_v, j);
	// 		pixel_center = t_vec3_add_vectors(&var->pixel00_loc, &temp_u);
	// 		pixel_center = t_vec3_add_vectors(&pixel_center, &temp_v);
	// 		ray_direction = t_vec3_subtract_vectors(&pixel_center, &var->camera_center);
	// 		ray = ray_create(&var->camera_center, &ray_direction);
	// 		pixel_color = ray_color(&ray, &world, var->camera_center);
	// 		write_color(pixel_color, var, j, i);
	// 	}
	// }
}

void printimage(void *param)
{
	t_var *var;
	var = param;

	hittable_list world;

	hittable_list_init(&world);
	// initobjects(&world);

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

int main(void)
{
	t_var var;

	var.camrerax = 0;
	var.camreray = 0;
	var.camreraz = 0;

	var.aspect_ratio = 16.0 / 9.0;
	var.image_height = SCREEN_WIDHT / (int)var.aspect_ratio;
	if (!(var.mlx = mlx_init(SCREEN_WIDHT, var.image_height, "MiniRT", true)))
	{
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(var.testimage = mlx_new_image(var.mlx, SCREEN_WIDHT, var.image_height)))
	{
		mlx_close_window(var.mlx);
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}

	if (mlx_image_to_window(var.mlx, var.testimage, 0, 0) == -1)
	{
		mlx_close_window(var.mlx);
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}

	printimage(&var); // If here. Cannot move camera while program is running
	hooks(&var);

	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	return (EXIT_SUCCESS);
}
// t_vec3 ray_color(t_var *var, const t_ray *r, const hittable_list *world)
// {
// 	(void)var;
// 	t_hit rec;
// 	if (world->hit(world, r, 0.001, INFINITY, &rec))
// 	{
// 		t_vec3 temp = t_vec3_create(rec.normal.e[0] + 1, rec.normal.e[1] + 1, rec.normal.e[2] + 1);
// 		return t_vec3_multiply_scalar(&temp, 0.5);
// 	}

// 	t_vec3 unit_direction = t_vec3_unit_vector(&r->dir);
// 	float t = 0.5 * (unit_direction.e[1] + 1.0);
// 	t_vec3 white = t_vec3_create(1.0, 1.0, 1.0);
// 	t_vec3 blue = t_vec3_create(0.5, 0.7, 1.0);
// 	t_vec3 temp_white = t_vec3_multiply_scalar(&white, 1.0 - t);
// 	t_vec3 temp_blue = t_vec3_multiply_scalar(&blue, t);
// 	return t_vec3_add_vectors(&temp_white, &temp_blue);
// }

// void initobjects(hittable_list objects)
// {
// 	// hittable_list objects;

// 	t_sphere s1 = sphere_create(t_vec3_create(-10, -10, -50), 5);
// 	t_sphere s2 = sphere_create(t_vec3_create(10, 10, -50), 5);
// 	t_sphere s3 = sphere_create(t_vec3_create(-10, 10, -50), 5);
// 	t_sphere s4 = sphere_create(t_vec3_create(10, -10, -50), 5);

// 	hittable_list_add(&objects, (t_hittable *)&s1);
// 	hittable_list_add(&objects, (t_hittable *)&s2);
// 	hittable_list_add(&objects, (t_hittable *)&s3);
// 	hittable_list_add(&objects, (t_hittable *)&s4);
// 	// return objects;
// }
