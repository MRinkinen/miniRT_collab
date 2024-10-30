/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 02:47:01 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 16:23:53 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# define WIDTH 1500
# define HEIGHT 600
# define MAX_OBJECTS 100
# define PI 3.1415927f
# define EPSILON 0.00001

# include "../libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"
# include "./parsing.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include <limits.h>
# include <float.h>
# include <assert.h>

// light sphere plane

typedef struct s_map		t_map;
typedef struct s_cylinders	t_cylinders;
typedef struct s_spheres	t_spheres;
typedef struct s_lights		t_lights;
typedef struct s_planes		t_planes;
typedef struct s_cameras	t_cameras;
typedef struct s_cylinder	t_cylinder;
typedef struct s_camera		t_camera;

typedef struct s_rotation_params
{
	float	cos_theta;
	float	sin_theta;
	float	one_minus_cos;
}	t_rotation_params;

typedef struct s_quad_coeffs
{
	float	a;
	float	b;
	float	c;
}	t_quad_coeffs;

typedef struct s_matrix
{
	int		rows;
	int		cols;
	float	data[4][4];
}	t_matrix;

typedef enum g_objecttype
{
	SPHERE,
	CYLINDER,
	PLANE
}	t_objecttype;

typedef struct s_tuple
{
	double	x;
	double	y;
	double	z;
	double	w;
}	t_tuple;

typedef struct s_ray
{
	t_tuple	origin;
	t_tuple	direction;
	t_tuple	px_center;
}	t_ray;

typedef struct s_t_values
{
	float	t0;
	float	t1;
}	t_t_values;

typedef struct s_cap_params
{
	const t_ray			*ray;
	const t_cylinder	*cylinder;
	float				*t;
	bool				hit;
}	t_cap_params;

typedef struct s_cylinder_params
{
	t_tuple	oc;
	t_tuple	direction;
	t_tuple	axis;
	float	radius;
	float	a;
	float	b;
	float	c;
}	t_cylinder_params;

typedef struct s_color
{
	float	r;
	float	g;
	float	b;
}	t_color;

typedef struct s_light
{
	t_tuple	position;
	t_color	color;
	float	brightness;
}	t_light;

typedef struct s_plane
{
	t_tuple		normal;
	t_matrix	*transform;
	t_matrix	*inverse_transform;
	t_matrix	*translation_matrix;
	t_matrix	*rotation_matrix;
	t_matrix	*scaling_matrix;
	t_color		color;
	t_tuple		center;
	t_tuple		orientation;
	t_tuple		point;
}	t_plane;

typedef struct s_cylinder
{
	t_tuple		normal;
	t_matrix	*transform;
	t_matrix	*inverse_transform;
	t_matrix	*translation_matrix;
	t_matrix	*rotation_matrix;
	t_matrix	*scaling_matrix;
	t_tuple		center;
	t_tuple		orientation;
	float		radius;
	float		height;
	t_color		color;
	float		minimum;
	float		maximum;
}	t_cylinder;

typedef struct s_sphere
{
	t_tuple		center;
	float		radius;
	t_color		color;
	t_matrix	*transform;
	t_matrix	*inverse_transform;
	t_matrix	*translation_matrix;
	t_matrix	*rotation_matrix;
	t_matrix	*scaling_matrix;

}	t_sphere;

typedef struct s_cam
{
	t_tuple	position;
	float	focal_length;
	t_tuple	focal;
	t_tuple	view_u;
	t_tuple	view_v;
	t_tuple	v_up_left_c;
	double	view_w;
	double	viewp_h;
	t_tuple	forward;
	t_tuple	right;
	t_tuple	up;
	t_tuple	delta_u;
	t_tuple	delta_v;
	t_tuple	loc_00;
}	t_cam;

typedef union u_objectdata
{
	t_sphere	sphere;
	t_cylinder	cylinder;
	t_plane		plane;
}	t_objectdata;

typedef struct s_object
{
	t_objecttype	type;
	t_objectdata	data;
}	t_object;

typedef struct s_var
{
	int			width;
	int			height;
	float		aspect_ratio;
	mlx_t		*mlx;
	mlx_image_t	*screenimage;
	float		image_width;
	float		image_height;
	int			screen_width;
	t_color		ambientl;
	t_cam		cam;
	t_light		*pointlights;
	int			num_lights;
	int			num_objects;
	t_object	*objects;
	t_color		temp_color;
	t_tuple		pixel_center;
	t_map		*map;
}	t_var;

void		print_matrix(t_matrix *matrix);

/*MLX*/
void		ft_hook(void *param);
void		hooks(t_var *var);
int			mlxinit(t_var *var);
void		printimage(void *param);
uint32_t	ft_pixel(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
void		write_color(t_color col, t_var *var, int x, int y);

/*Init*/

int			init_light(t_var *var, t_map *map);
int			init_ambient_color(t_var *var, t_map *map);
int			init_objects(t_var *var, t_map *map);

/*Free*/
void		free_cameras(t_camera *camera);
void		free_lights(t_lights *lights);
void		free_spheres(t_spheres *sphere);
void		free_planes(t_planes *plane);
void		free_cylinders(t_cylinders *cylinder);
int			free_scene(t_var *var);

/*Camera*/

int			initialize_camera(t_var *var, t_map *map);

/*Color*/

t_color		color_add(t_color c1, t_color c2);
t_color		t_color_create(int r, int g, int b);
t_color		subtract_colors(t_color c1, t_color c2);
t_color		multiply_color_scalar(t_color color, float scalar);
t_color		multiply_colors(t_color a, t_color b);

/*Sphere*/
void		create_spheres(t_var *var, t_map *map, int *obj_index);
t_tuple		calculate_sphere_normal(const t_sphere *sphere, \
			const t_tuple *point);
bool		intersect_sphere(const t_ray *ray, \
			const t_sphere *sphere, float *t);

/*Cylinder*/
void		create_cylinders(t_var *var, t_map *map, int *obj_index);
t_tuple		calculate_cylinder_normal(t_cylinder *cylinder, t_tuple *point);
bool		intersect_cylinder(t_ray *ray, \
			const t_cylinder *cylinder, float *t);
void		set_cylinder_params(const t_ray *ray, const t_cylinder *cylinder, \
			t_cylinder_params *params);
bool		check_cylinder_height(const t_ray *ray, const t_cylinder *cylinder, \
			float *t, const t_t_values *t_vals);

/*Plane*/
void		create_planes(t_var *var, t_map *map, int *obj_index);
int			intersect_plane(t_ray *ray, t_plane *plane, float *t);

/*Light*/
t_light		light_create(t_tuple position, t_color intensity, float brightness);
bool		is_in_shadow(const t_tuple *point, t_light *light, \
			t_object *objects, int num_objects);
t_color		calculate_phong_lighting(t_var *var, const t_tuple *point, \
			const t_tuple *normal, const t_tuple *view_dir);

/*Render*/
bool		intersect_object(t_ray *ray, t_object *object, float *t);
bool		find_closest_intersection(t_var *var, t_ray *ray, \
			t_object **closest_object, float *closest_t);
t_tuple		calculate_normal(t_object *object, t_tuple *point);
t_color		shade_pixel(t_var *var, t_ray *r, \
			t_object *closest_object, float closest_t);
//void		process_pixel(t_var *var, int x, int y);
void		process_pixel(t_var *var, int x, int y);
void		printimage(void *param);

/*Matrix*/
void		initialize_identity(t_matrix *m);
void		fill_submatrix(t_matrix *sub_m, const t_matrix *m, \
			int remove_row, int remove_col);
t_matrix	*allocate_submatrix(int new_rows, int new_cols);
t_matrix	*allocate_matrix(int rows, int cols);
void		multiply_elements(t_matrix *result, t_matrix *a, t_matrix *b);
t_matrix	*tuple_to_matrix(t_tuple *t);
t_matrix	*handle_axis_length_case(t_tuple v1, t_tuple v2);
t_matrix	*handle_special_case(t_tuple v2);
t_matrix	*rotation_around_axis(t_tuple axis, float angle);
t_matrix	*rotation_z(float radians);
t_matrix	*rotation_x(float radians);
t_matrix	*rotation_y(float radians);
t_matrix	*scaling(float x, float y, float z);
t_matrix	*inverse_scaling(float x, float y, float z);
t_matrix	*reflective_scaling(float x, float y, float z);
t_matrix	*submatrix(const t_matrix *m, int remove_row, int remove_col);
void		fill_inverse(t_matrix *inverse_m, t_matrix *adjugate_m, float det);
void		fill_rotation_values(float *values, t_tuple axis, \
			t_rotation_params params);
t_matrix	*inverse(t_matrix *m);
t_matrix	*inverse_translation(t_matrix *transform);
t_matrix	*translation(float x, float y, float z);
t_matrix	*cofactor_matrix(const t_matrix *m);
t_matrix	*identity_matrix(void);
t_matrix	*transpose(t_matrix *m);
t_matrix	*t_matrix_multiply(t_matrix *a, t_matrix *b);
t_matrix	*create_4x4_matrix(float values[16]);
t_matrix	*rotation_from_normal(t_tuple normal);

/*Tuple*/
t_tuple		position(t_ray r, double t);
t_tuple		apply_transformation(t_matrix *transformation, t_tuple *point);
t_tuple		matrix_to_tuple(t_matrix *m);
t_tuple		tuple_add(t_tuple t1, t_tuple t2);
t_tuple		tuple_subtract(t_tuple t1, t_tuple t2);
t_tuple		subtract_vector_from_point(t_tuple point, t_tuple vector);
t_tuple		negate_vector(t_tuple v);
t_tuple		negate_tuple(t_tuple t);
t_tuple		tuple_multiply(t_tuple t, double scalar);
t_tuple		tuple_divide(t_tuple t, double scalar);
t_tuple		point(double x, double y, double z);
t_tuple		vector(double x, double y, double z);
t_tuple		tuple(double x, double y, double z, double w);
t_tuple		normalize(t_tuple v);
t_tuple		cross(t_tuple a, t_tuple b);
t_tuple		get_default_normal(void);
t_tuple		get_arbitrary_axis(t_tuple v1);
float		cofactor(const t_matrix *m, int row, int col);
float		minor(const t_matrix *m, int row, int col);

/*Ray*/
t_ray		ray(t_tuple origin, t_tuple direction);
t_ray		generate_ray_for_pixel(t_var *var, int x, int y);
void		print_color(t_color c);
bool		equal(double a, double b);
bool		compare_colors(t_color c1, t_color c2);
bool		is_point(t_tuple t);
bool		is_vector(t_tuple t);
bool		tuple_equal(t_tuple t1, t_tuple t2);
float		minor(const t_matrix *m, int row, int col);
float		determinant(const t_matrix *m);
float		determinant_2x2(const t_matrix *m);
float		determinant_3x3(const t_matrix *m);
float		determinant_4x4(const t_matrix *m);
float		t_matrix_get(t_matrix *m, int row, int col);
float		magnitude(t_tuple v);
bool		magnitude_equal(t_tuple v, double expected_magnitude);
float		dot(t_tuple a, t_tuple b);
float		cofactor(const t_matrix *m, int row, int col);
bool		is_invertible(t_matrix *m);
bool		check_special_case(t_tuple v2);

#endif