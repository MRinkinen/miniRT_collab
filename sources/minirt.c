#include "../includes/minirt.h"

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void printsquare(void *param)
{
	t_var *var;
	var = param;
	(void)var;
	for (int j = 0; j < SCREEN_HEIGHT; j++)
	{
		for (int i = 0; i < SCREEN_WIDHT; i++)
		{
			t_vec3 pixel_color = t_vec3_create((double)i / (SCREEN_WIDHT - 1), (double)j / (SCREEN_HEIGHT - 1), 0);
			write_color(pixel_color, var, j, i);
		}
	}
}

void ft_hook(void *param)
{
	t_var *var;

	var = param;

	if (mlx_is_key_down(var->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(var->mlx);
}

// -----------------------------------------------------------------------------

void hooks(t_var *var)
{
	mlx_loop_hook(var->mlx, printsquare, var);
	mlx_loop_hook(var->mlx, ft_hook, var);
}

int main(void)
{
	t_var var;

	var.aspect_ratio = 16.0 / 9.0;
	var.image_width = 400;

	var.image_height = var.image_width / (int)var.aspect_ratio;
	// image_height = (image_height < 1) ? 1 : image_height;
	var.focal_length = 1.0;
	var.viewport_height = 2.0;
	var.viewport_width = var.viewport_height * (double)var.image_width / (double)var.image_height;
	var.camera_center = t_vec3_create(0, 0, 0);
	var.viewport_u = t_vec3_create(var.viewport_width, 0, 0);
	var.viewport_v = t_vec3_create(0, -var.viewport_height, 0);

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
