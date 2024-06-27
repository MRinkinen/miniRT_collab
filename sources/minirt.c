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
}

t_vec3 color_add(t_vec3 c1, t_vec3 c2) {
    t_vec3 result;
    result.e[0] = c1.e[0] + c2.e[0];
    result.e[1] = c1.e[1] + c2.e[1];
    result.e[2] = c1.e[2] + c2.e[2];
    return result;
}

// Multiplies each component of a color (vector) by a scalar
t_vec3 color_multiply_scalar(t_vec3 c, double s) {
    t_vec3 result;
    result.e[0] = c.e[0] * s;
    result.e[1] = c.e[1] * s;
    result.e[2] = c.e[2] * s;
    return result;
}

t_vec3 ray_color(const t_ray* r)
{
    t_vec3 result;
	if (hit_sphere((t_vec3){{0, 0, -1}}, 0.2, *r))
	{
		result = t_vec3_create(1,0,0);
        return result;
	}

    t_vec3 unit_direction = t_vec3_unit_vector(&r->dir); // Assuming direction is a function-like macro or a direct struct member access
    double a = 0.5 * (unit_direction.e[1] + 1.0); // Assuming y is a direct member of struct vec3
    t_vec3 white = {1.0, 1.0, 1.0};
    t_vec3 blue_sky = {0.5, 0.7, 1.0};

	result = color_add(color_multiply_scalar(white, (1.0 - a)), color_multiply_scalar(blue_sky, a));

    return result;
}

void testifunk(void *param)
{
	t_var *var;
 	var = param;
 	//(void)var;

    // Image
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)image_width / image_height);
    t_vec3 camera_center = t_vec3_create(0, 0, 0);

    t_vec3 viewport_u = t_vec3_create(viewport_width, 0, 0);
    t_vec3 viewport_v = t_vec3_create(0, -viewport_height, 0);

    t_vec3 pixel_delta_u = t_vec3_divide_scalar(&viewport_u, image_width);
    t_vec3 pixel_delta_v = t_vec3_divide_scalar(&viewport_v, image_height);

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
    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = 0; j < SCREEN_HEIGHT; j++) {
        fprintf(stderr, "\rScanlines remaining: %d ", SCREEN_HEIGHT - j);
        fflush(stderr);
        for (int i = 0; i < SCREEN_WIDHT; i++) {
            t_vec3 temp1 = t_vec3_multiply_scalar(&pixel_delta_u, i);
            t_vec3 temp2 = t_vec3_multiply_scalar(&pixel_delta_v, j);
			// Step 1: Add pixel00_loc and temp1, store the result in a temporary variable.
			t_vec3 temp_result = t_vec3_add_vectors(&pixel00_loc, &temp1);
			// Step 2: Add the result of step 1 (temp_result) to temp2.
			t_vec3 pixel_center = t_vec3_add_vectors(&temp_result, &temp2);
            t_vec3 ray_direction = t_vec3_subtract_vectors(&pixel_center, &camera_center);
            t_ray r = ray_create(&camera_center, &ray_direction);

            t_vec3 pixel_color = ray_color(&r);
            write_color(pixel_color, var, j, i);
        }
    }

    fprintf(stderr, "\rDone.                 \n");
}

void hooks(t_var *var)
{
	mlx_loop_hook(var->mlx, testifunk, var);
	mlx_loop_hook(var->mlx, ft_hook, var);
}

int main(void)
{
	t_var var;

	if (!(var.mlx = mlx_init(SCREEN_WIDHT, SCREEN_HEIGHT, "MiniRT", false)))
	{
		ft_printf("%s", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(var.testimage = mlx_new_image(var.mlx, SCREEN_WIDHT, SCREEN_HEIGHT)))
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
