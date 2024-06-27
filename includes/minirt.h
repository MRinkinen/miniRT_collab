#ifndef MINIRT_H
#define MINIRT_H

#include <stdbool.h>
#include <stdio.h> // DELETE BEFORE RETURN!!
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include "../libft/libft.h"
#include "../MLX42/include/MLX42/MLX42.h"

#define SCREEN_HEIGHT 1000
#define SCREEN_WIDHT 1000

typedef struct s_vec
{
	double e[3];
} t_vec3;

typedef struct s_ray
{
	t_vec3 orig;
	t_vec3 dir;
} t_ray;

typedef struct s_var
{
	mlx_t *mlx;
	t_vec3 *vector;

	double aspect_ratio;
	int image_width;
	int image_height;

	// CAMERA
	double focal_length;
	double viewport_height;
	double viewport_width;
	t_vec3 camera_center;

	t_vec3 viewport_u;
	t_vec3 viewport_v;

	// auto pixel_delta_u = viewport_u / image_width;
	// auto pixel_delta_v = viewport_v / image_height;

	mlx_image_t *testimage;
	// mlx_image_t		*exit;
	// mlx_image_t		*wall;
	// mlx_image_t		*ground;
	// mlx_image_t		*collect;
	// mlx_image_t		*player_right;
	// mlx_image_t		*player_left;
	// mlx_texture_t	*p_right;
	// mlx_texture_t	*p_left;
	// mlx_texture_t	*exit_tex;
	// mlx_texture_t	*wall_tex;
	// mlx_texture_t	*ground_tex;
	// mlx_texture_t	*collect_tex;
} t_var;

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
void write_color(t_vec3 vec, t_var *var, int x, int y);

/*Vector 3*/
t_vec3 t_vec3_create(double e0, double e1, double e2);
t_vec3 t_vec3_multiply_scalar(const t_vec3 *v, double t);
t_vec3 t_vec3_add_vectors(const t_vec3 *u, const t_vec3 *v);
#endif
