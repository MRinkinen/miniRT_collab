#ifndef MINIRT_H
# define MINIRT_H

# include <stdbool.h>
# include <stdio.h> // DELETE BEFORE RETURN!!
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include <limits.h>
# include <float.h>
# include <assert.h> // Only for testing purposes, remove from final version
# include "../libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"
# include "./parsing.h"
//#include "./test_functions.h"

# define WIDTH 1500
# define HEIGHT 600
# define MAX_OBJECTS 100
# define PI 3.1415927f
//# define PI 3.141592653589793
//#define EPSILON 0.9
# define EPSILON 0.00001

typedef struct s_map	t_map;
typedef struct s_cylinders t_cylinders;

typedef struct s_rotation_params
{
	float	cos_theta;
	float	sin_theta;
	float	one_minus_cos;
}	t_rotation_params;

typedef struct s_quad_coeffs
{
    float a;
    float b;
    float c;
} t_quad_coeffs;

typedef struct
{
    int rows;
    int cols;
    float data[4][4]; // This can hold up to a 4x4 matrix, adjust the size if necessary
} t_matrix;

// typedef struct
// {
//     double red;
//     double green;
//     double blue;
// } t_color;

/* Above is all test_functions.h definitions*/

typedef enum e_objecttype
{
    SPHERE,
    CYLINDER,
    PLANE
} t_objecttype;

typedef struct s_tuple
{
    double x, y, z, w;
}         t_tuple;

typedef struct s_ray
{
    t_tuple origin;
    t_tuple direction;
	t_tuple px_center;
} t_ray;

typedef struct s_color
{
	float r;
	float g;
	float b;
} t_color;

typedef struct s_light
{
    t_tuple position;    // Position of the light in the scene
    t_color color;   // Intensity (color) of the light
    float brightness;    // Brightness ratio in the range [0.0, 1.0]
} t_light;

typedef struct s_plane
{
	t_tuple	  normal;
    t_matrix *transform;
    t_matrix *inverse_transform;
	t_matrix *translation_matrix;
	t_matrix *rotation_matrix;
	t_matrix *scaling_matrix;
    t_color   color;
	t_tuple	  center;
	t_tuple   orientation;
	t_tuple   point;
} t_plane;

typedef struct s_cylinder
{
	t_tuple	  	normal;
    t_matrix 	*transform;
    t_matrix 	*inverse_transform;
	t_matrix 	*translation_matrix;
	t_matrix 	*rotation_matrix;
	t_matrix 	*scaling_matrix;
	t_tuple 	center;		// Center of the base of the cylinder
	t_tuple 	orientation; // Orientation of the cylinder (usually represented by a vector)
	float 		radius;		// Radius of the cylinder
	float 		height;		// Height of the cylinder
	t_color 	color;		// t_color of the cylinder
	float 		minimum;
	float 		maximum;
} t_cylinder;

typedef struct s_sphere
{
    t_tuple center;  // Sphere's center (position)
    float radius;    // Sphere's radius
    t_color color;   // Sphere's color

    // Transformation matrices
    t_matrix *transform;        // Combined transformation matrix
    t_matrix *inverse_transform; // Inverse of the combined transformation matrix

    // Optional: Separate transformation components if needed
    t_matrix *translation_matrix; // Translation matrix
    t_matrix *rotation_matrix;    // Rotation matrix
    t_matrix *scaling_matrix;     // Scaling matrix

} t_sphere;

typedef struct s_cam
{
	t_tuple position;           // Camera position
	float focal_length;         // Focal length based on fov
	t_tuple focal;			  	// Focal point
	t_tuple view_u; 			// Camera basis vector (right)
	t_tuple view_v; 			// Camera basis vector (up)
	t_tuple v_up_left_c;		// Upper-left corner of the viewport (viewport_up_l)
	double view_w;       		// Width of the viewport
	double viewp_h;      		// Height of the viewport
	t_tuple forward;            // Forward direction (camera_f)
	t_tuple right;              // Right direction (camera_r)
	t_tuple up;                 // Up direction (camera_up)
	t_tuple delta_u;            // Delta u
	t_tuple delta_v;            // Delta v
	t_tuple loc_00;        		// Location of the 00 pixel
} t_cam;

typedef union u_objectdata
{
    t_sphere sphere;
    t_cylinder cylinder;
    t_plane plane;
} u_objectdata;

typedef struct s_object
{
    t_objecttype type;
    u_objectdata data;
} t_object;

typedef struct s_var
{
	int			width;
	int			height;
	float		aspect_ratio;
	mlx_t		*mlx;
	mlx_image_t *screenimage;
	float		image_width;
	float		image_height;
	int			screen_width;
	t_color		ambientl;
	t_cam 		cam;
	t_light 	*pointlights;
	int			num_lights;
	int 		num_objects;
	t_object 	*objects;
	t_color 	temp_color;
	t_tuple 	pixel_center;		// Center of the pixel
} t_var;

void print_matrix(t_matrix *matrix);


/*MLX*/
void ft_hook(void *param);
void hooks(t_var *var);
int mlxinit(t_var *var);
void printimage(void *param);
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
void write_color(t_color col, t_var *var, int x, int y);

/*Init*/

void initialize_scene(t_var *var, t_map *map);
void init_light(t_var *var, t_map *map);
void init_ambient_color(t_var *var, t_map *map);

/*Free*/
int free_scene(t_var *var);

/*Camera*/

void initialize_camera(t_var *var, t_cam *camera, t_map *map);

/*Color*/

t_color color_add(t_color c1, t_color c2);
t_color t_color_create(int r, int g, int b);
t_color subtract_colors(t_color c1, t_color c2);
t_color multiply_color_scalar(t_color color, float scalar);
t_color multiply_colors(t_color a, t_color b);
bool intersect_object(const t_ray *ray, const t_object *object, float *t);

/*Sphere*/
t_sphere sphere_create(t_tuple center, float radius, t_color col);
t_tuple calculate_sphere_normal(const t_sphere *sphere, const t_tuple *point);
bool intersect_sphere(const t_ray *ray, const t_sphere *sphere, float *t);


/*Cylinder*/
t_cylinder cylinder_create(t_var *var, t_map *map, int obj_index, t_cylinders *current_cylinder);
//t_cylinder cylinder_create(t_tuple center, float radius, float height, t_color color, t_tuple orientation);
t_tuple calculate_cylinder_normal(const t_cylinder *cylinder, const t_tuple *point);
bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t);

/*Plane*/
t_plane plane_create(t_tuple center, t_color color, t_tuple orientation);
bool intersect_plane(const t_ray *ray, const t_plane *plane, float *t);


/*Light*/
t_light light_create(t_tuple position, t_color intensity, float brightness);
bool is_in_shadow(const t_tuple *point, const t_light *light, const t_object *objects, int num_objects) ;
t_color calculate_phong_lighting(t_var *var, const t_tuple *point, const t_tuple *normal, const t_tuple *view_dir);

/*Matrix*/
t_matrix    *tuple_to_matrix(t_tuple *t);
t_matrix    *shearing(float xy, float xz, float yx, float yz, float zx, float zy);
t_matrix    *rotation_z(float radians);
t_matrix    *rotation_x(float radians);
t_matrix    *rotation_y(float radians);
t_matrix    *scaling(float x, float y, float z);
t_matrix    *inverse_scaling(float x, float y, float z);
t_matrix    *reflective_scaling(float x, float y, float z);
t_matrix    *submatrix(const t_matrix *m, int remove_row, int remove_col);
t_matrix    *inverse(t_matrix *m);
t_matrix    *inverse_translation(t_matrix *transform);
t_matrix    *translation(float x, float y, float z);
t_matrix*   cofactor_matrix(const t_matrix *m);
t_matrix*   identity_matrix();
t_matrix*   transpose(t_matrix *m);
t_matrix*   t_matrix_multiply(t_matrix *a, t_matrix *b);
t_matrix    *create_2x2_matrix(float a, float b, float c, float d);
t_matrix    *create_3x3_matrix(float a, float b, float c,
                           float d, float e, float f,
                           float g, float h, float i);
t_matrix 	*create_4x4_matrix(float values[16]);
t_matrix* 	rotation_from_normal(t_tuple normal);

/*Tuple*/
t_tuple       position(t_ray r, double t);
//t_tuple       apply_transformation(t_matrix *transformation, t_tuple *point);
t_tuple 	  apply_transformation(t_matrix *transformation, t_tuple *point);
t_tuple       matrix_to_tuple(t_matrix *m);
t_tuple       tuple_add(t_tuple t1, t_tuple t2);
t_tuple       tuple_subtract(t_tuple t1, t_tuple t2);
t_tuple       subtract_vector_from_point(t_tuple point, t_tuple vector);
t_tuple       negate_vector(t_tuple v);
t_tuple       negate_tuple(t_tuple t);
t_tuple       tuple_multiply(t_tuple t, double scalar);
t_tuple       tuple_divide(t_tuple t, double scalar);
t_tuple       point(double x, double y, double z);
t_tuple       vector(double x, double y, double z);
t_tuple       tuple(double x, double y, double z, double w);
t_tuple       normalize(t_tuple v);
t_tuple       cross(t_tuple a, t_tuple b);

/*Ray*/
t_ray         ray(t_tuple origin, t_tuple direction);
t_ray generate_ray_for_pixel(t_var *var, int x, int y);

void        print_color(t_color c);
bool        equal(double a, double b);
bool        compare_colors(t_color c1, t_color c2);
bool        is_point(t_tuple t);
bool        is_vector(t_tuple t);
bool        tuple_equal(t_tuple t1, t_tuple t2);
float       minor(const t_matrix *m, int row, int col);
float       determinant(const t_matrix *m);
float       determinant_2x2(const t_matrix *m);
float       determinant_3x3(const t_matrix *m);
float       t_matrix_get(t_matrix *m, int row, int col);
float      magnitude(t_tuple v);
bool        magnitude_equal(t_tuple v, double expected_magnitude);
float      dot(t_tuple a, t_tuple b);
float       cofactor(const t_matrix *m, int row, int col);
bool        is_invertible(t_matrix *m);

#endif
