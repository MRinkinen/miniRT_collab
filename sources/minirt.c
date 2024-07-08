#include "../includes/minirt.h"

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void ft_hook(void *param)
{
	t_var *var;

	var = param;

	if (mlx_is_key_down(var->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(var->mlx);
	if (mlx_is_key_down(var->mlx, MLX_KEY_W) && mlx_is_key_down(var->mlx, MLX_KEY_LEFT_SHIFT))
		var->camreraz += 1;
	else if (mlx_is_key_down(var->mlx, MLX_KEY_W))
		var->camrerax -= 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_S) && mlx_is_key_down(var->mlx, MLX_KEY_LEFT_SHIFT))
		var->camreraz -= 1;
	else if (mlx_is_key_down(var->mlx, MLX_KEY_S))
		var->camrerax += 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_A))
		var->camreray += 1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_D))
		var->camreray -= 1;
}

t_vec3 color_add(t_vec3 c1, t_vec3 c2)
{
	t_vec3 result;
	result.e[0] = c1.e[0] + c2.e[0];
	result.e[1] = c1.e[1] + c2.e[1];
	result.e[2] = c1.e[2] + c2.e[2];
	return result;
}

// Multiplies each component of a color (vector) by a scalar
t_vec3 color_multiply_scalar(t_vec3 c, double s)
{
	t_vec3 result;
	result.e[0] = c.e[0] * s;
	result.e[1] = c.e[1] * s;
	result.e[2] = c.e[2] * s;
	return result;
}

// t_vec3 ray_color(t_var *var, const t_ray *r)
// {

// 	  t_hit rec;
//     if (world->hit(world, r, 0.001, INFINITY, &rec)) {
//         vec3 temp = vec3_create(rec.normal.e[0] + 1, rec.normal.e[1] + 1, rec.normal.e[2] + 1);
//         return vec3_multiply_scalar(&temp, 0.5);
//     }
// 	// (void)var;
// 	// static double test = 0;
// 	// static double test2 = 100;

// 	// t_vec3 sphere_center = t_vec3_create(0, test, test2);
// 	// // test = test + 0.000001;
// 	// // test2 = test2 - 0.000001;
// 	// double t = hit_sphere(&sphere_center, 12.6, r);
// 	// if (t > 0.0) // voi olla viturallaan
// 	// {
// 	// 	t_vec3 point_at_t = ray_at(r, t);
// 	// 	t_vec3 temp_vec = t_vec3_subtract_vectors(&point_at_t, &sphere_center);
// 	// 	t_vec3 normal = t_vec3_unit_vector(&temp_vec);
// 	// 	t_vec3 temp_N = t_vec3_create(normal.e[0] + 1, normal.e[1] + 1, normal.e[2] + 1);
// 	// 	t_vec3 N = t_vec3_multiply_scalar(&temp_N, 0.5);
// 	// 	return N;
// 	// }
// 	t_vec3 unit_direction = t_vec3_unit_vector(&r->dir);
// 	double a = 0.5 * (unit_direction.e[1] + 1.0);
// 	t_vec3 white = t_vec3_create(1.0, 1.0, 1.0);
// 	t_vec3 blue = t_vec3_create(0.5, 0.7, 1.0);
// 	t_vec3 temp_white = t_vec3_multiply_scalar(&white, 1.0 - a);
// 	t_vec3 temp_blue = t_vec3_multiply_scalar(&blue, a);
// 	t_vec3 blended_color = t_vec3_add_vectors(&temp_white, &temp_blue);
// 	return blended_color;
// }

t_vec3 ray_color(t_var *var, const t_ray *r, const t_hittable *world)
{
	(void)var;
	t_hit rec;
	if (world->hit(world, r, 0.001, INFINITY, &rec))
	{
		t_vec3 temp = t_vec3_create(rec.normal.e[0] + 1, rec.normal.e[1] + 1, rec.normal.e[2] + 1);
		return t_vec3_multiply_scalar(&temp, 0.5);
	}

	t_vec3 unit_direction = t_vec3_unit_vector(&r->dir);
	double t = 0.5 * (unit_direction.e[1] + 1.0);
	t_vec3 white = t_vec3_create(1.0, 1.0, 1.0);
	t_vec3 blue = t_vec3_create(0.5, 0.7, 1.0);
	t_vec3 temp_white = t_vec3_multiply_scalar(&white, 1.0 - t);
	t_vec3 temp_blue = t_vec3_multiply_scalar(&blue, t);
	return t_vec3_add_vectors(&temp_white, &temp_blue);
}

void testifunk(void *param)
{
	t_var *var;
	var = param;

	t_sphere s = sphere_create(t_vec3_create(0, 0, -80), 50);
	t_hittable *world = (t_hittable *)&s;

	double focal_length = 1.0;
	double viewport_height = 2.0;
	double viewport_width = viewport_height * ((double)SCREEN_WIDHT / var->image_height);
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
		// fprintf(stderr, "\rScanlines remaining: %d ", var->image_height - j);
		// fflush(stderr);
		for (int i = 0; i < SCREEN_WIDHT; i++)
		{
			t_vec3 temp1 = t_vec3_multiply_scalar(&pixel_delta_u, i);
			t_vec3 temp2 = t_vec3_multiply_scalar(&pixel_delta_v, j);
			// Step 1: Add pixel00_loc and temp1, store the result in a temporary variable.
			t_vec3 temp_result = t_vec3_add_vectors(&pixel00_loc, &temp1);
			// Step 2: Add the result of step 1 (temp_result) to temp2.
			t_vec3 pixel_center = t_vec3_add_vectors(&temp_result, &temp2);
			t_vec3 ray_direction = t_vec3_subtract_vectors(&pixel_center, &camera_center);
			t_ray r = ray_create(&camera_center, &ray_direction);

			t_vec3 pixel_color = ray_color(var, &r, world);
			write_color(pixel_color, var, j, i);
		}
	}

	// fprintf(stderr, "\rDone.                 \n");
}

void hooks(t_var *var)
{
	mlx_loop_hook(var->mlx, testifunk, var);
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
	hooks(&var);

	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	return (EXIT_SUCCESS);
}
