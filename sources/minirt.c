/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/15 13:34:51 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void ft_hook(void *param)
{
	t_var *var;

	var = param;

	if (mlx_is_key_down(var->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(var->mlx);
	if (mlx_is_key_down(var->mlx, MLX_KEY_W) && mlx_is_key_down(var->mlx, MLX_KEY_LEFT_SHIFT))
		var->camreraz -= 1;
	else if (mlx_is_key_down(var->mlx, MLX_KEY_W))
		var->camrerax += 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_S) && mlx_is_key_down(var->mlx, MLX_KEY_LEFT_SHIFT))
		var->camreraz += 1;
	else if (mlx_is_key_down(var->mlx, MLX_KEY_S))
		var->camrerax -= 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_A))
		var->camreray -= 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_D))
		var->camreray += 1;
}

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

t_color ray_color(const t_ray *r, const hittable_list *world, t_vec3 camera_pos)
{
	t_hit rec;

	if (hittable_list_hit(world, r, 0, INFINITY, &rec)) // Muuteltu rajusti!!
	{
		float distance = calculate_distance(camera_pos, r->dir);

		// Define attenuation factors
		float constant = 1.0;
		float linear = 0.09;
		float quadratic = 0.032;

		// Calculate the attenuation based on the distance
		float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

		// Apply the attenuation to the object's color
		t_color temp = t_color_create(rec.normal.x, rec.normal.y, rec.normal.z);
		t_color final_color = color_multiply_scalar(temp, attenuation);
		//  printf("%f\n", temp.e[0]);
		//   t_vec3 temp = t_vec3_create(255, 45, 68);
		return (final_color);
		// return (color_multiply_scalar(temp, t_vec3_magnitude(&r->dir) * 0.50)); // Ei loydy hyvaa lukua!!!
	}

	t_vec3 unit_direction = t_vec3_unit_vector(&r->dir);
	float t = 0.5 * (unit_direction.y + 1.0);
	t_color white = t_color_create(1.0, 1.0, 1.0);
	t_color blue = t_color_create(0.5, 0.7, 1.0);
	t_color temp_white = color_multiply_scalar(white, 1.0 - t);
	t_color temp_blue = color_multiply_scalar(blue, t);
	return color_add(temp_white, temp_blue);
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
void testifunk(void *param)
{
	t_var *var;
	var = param;

	// hittable_list objects;
	// hittable_list_init(&objects);

	// initobjects(objects);
	// t_sphere s = sphere_create(t_vec3_create(0, 0, -30), 20);
	// t_hittable *world = (t_hittable *)&s;

	hittable_list world;
	hittable_list_init(&world);

	t_sphere s1 = sphere_create(t_vec3_create(20, 20, -220), 20, t_color_create(100, 100, 100));
	t_sphere s2 = sphere_create(t_vec3_create(20, -20, -180), 20, t_color_create(100, 100, 100));
	t_sphere s3 = sphere_create(t_vec3_create(-20, -20, -160), 20, t_color_create(100, 100, 100));
	t_sphere s4 = sphere_create(t_vec3_create(-20, 20, -100), 20, t_color_create(100, 100, 100));
	//  t_sphere s1b = sphere_create(t_vec3_create(-10, -10, -100), 10);
	//  t_sphere s2b = sphere_create(t_vec3_create(10, 10, -100), 10);
	//  t_sphere s3b = sphere_create(t_vec3_create(-10, 10, -100), 10);
	//  t_sphere s4b = sphere_create(t_vec3_create(10, -10, -100), 10);

	hittable_list_add(&world, (t_hittable *)&s1);
	hittable_list_add(&world, (t_hittable *)&s2);
	hittable_list_add(&world, (t_hittable *)&s3);
	hittable_list_add(&world, (t_hittable *)&s4);
	//  hittable_list_add(&world, (t_hittable *)&s1b);
	//  hittable_list_add(&world, (t_hittable *)&s2b);
	//  hittable_list_add(&world, (t_hittable *)&s3b);
	//  hittable_list_add(&world, (t_hittable *)&s4b);

	float focal_length = 1.0;
	float viewport_height = 1.0; // Jos ei 1, niin pallukat soikeita
	float viewport_width = viewport_height * ((float)SCREEN_WIDHT / var->image_height);
	t_vec3 camera_center = t_vec3_create(var->camrerax, var->camreray, var->camreraz);

	t_vec3 viewport_u = t_vec3_create(viewport_width, 0, 0);
	t_vec3 viewport_v = t_vec3_create(0, -viewport_height, 0);

	t_vec3 pixel_delta_u = t_vec3_divide_scalar(&viewport_u, SCREEN_WIDHT);
	t_vec3 pixel_delta_v = t_vec3_divide_scalar(&viewport_v, var->image_height);

	t_vec3 temp1 = t_vec3_create(0, 0, -focal_length);
	t_vec3 temp2 = t_vec3_divide_scalar(&viewport_u, 2);
	t_vec3 temp3 = t_vec3_divide_scalar(&viewport_v, 2);
	t_vec3 temp4 = t_vec3_add_vectors(&temp1, &temp2);
	t_vec3 temp5 = t_vec3_add_vectors(&temp4, &temp3);
	t_vec3 viewport_upper_left = t_vec3_subtract_vectors(&camera_center, &temp5);

	t_vec3 temp = t_vec3_add_vectors(&pixel_delta_u, &pixel_delta_v);
	// Assuming t_vec3_multiply_scalar returns a t_vec3 by value.
	t_vec3 temp_result = t_vec3_multiply_scalar(&temp, 0.5);
	// Now, pass the address of temp_result to t_vec3_add_vectors.
	t_vec3 pixel00_loc = t_vec3_add_vectors(&viewport_upper_left, &temp_result);
	// Render
	// printf("P3\n%d %d\n255\n", SCREEN_WIDHT, var->image_height);

	for (int j = 0; j < var->image_height; j++)
	{
		for (int i = 0; i < SCREEN_WIDHT; i++)
		{
			t_vec3 temp_u = t_vec3_multiply_scalar(&pixel_delta_u, i);
			t_vec3 temp_v = t_vec3_multiply_scalar(&pixel_delta_v, j);
			t_vec3 pixel_center = t_vec3_add_vectors(&pixel00_loc, &temp_u);
			pixel_center = t_vec3_add_vectors(&pixel_center, &temp_v);
			t_vec3 ray_direction = t_vec3_subtract_vectors(&pixel_center, &camera_center);
			t_ray r = {camera_center, ray_direction};

			t_color pixel_color = ray_color(&r, &world, camera_center);
			write_color(pixel_color, var, j, i);
		}
	}

	// fprintf(stderr, "\rDone.                 \n");
}

void hooks(t_var *var)
{
	mlx_loop_hook(var->mlx, testifunk, var); // If here. Can move camera while program is running
	mlx_loop_hook(var->mlx, ft_hook, var);
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
	// testifunk(&var); // If here. Cannot move camera while program is running
	hooks(&var);

	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	return (EXIT_SUCCESS);
}
