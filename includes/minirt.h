#ifndef MINIRT_H
#define MINIRT_H

#include <stdbool.h>
#include <stdio.h> // DELETE BEFORE RETURN!!
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <assert.h> // Only for testing purposes, remove from final version
#include "../libft/libft.h"
#include "../MLX42/include/MLX42/MLX42.h"
#include "./parsing.h"
//#include "./test_functions.h"

#define SCREEN_HEIGHT 600 // Not in use
#define SCREEN_WIDTH 1500
#define MAX_OBJECTS 100
#define PI 3.141592653589793
//#define EPSILON 0.9
#define EPSILON 0.00001

typedef struct s_map	t_map;

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

typedef struct s_tuple
{
    double x, y, z, w;
}         t_tuple;

typedef struct s_ray
{
    t_tuple origin;
    t_tuple direction;
} t_ray;

typedef struct s_color
{
	float r;
	float g;
	float b;
} t_color;

typedef struct s_vec
{
	float x;
	float y;
	float z;
} t_vec3;

typedef struct s_hit
{
	t_vec3 p;
	t_vec3 normal;
	float t;
	bool front_face;
	t_color color;
} t_hit;

struct hittable;
void set_face_normal(t_hit *rec, const t_ray *r, const t_vec3 *outward_normal);
typedef bool (*hit_func)(const struct hittable *, const t_ray *, float, float, t_hit *);

typedef struct hittable
{
	hit_func hit;
} t_hittable;

typedef struct
{
	t_hittable *objects[MAX_OBJECTS];
	int size;
} hittable_list;

void hittable_list_init(hittable_list *list);
void hittable_list_clear(hittable_list *list);
void hittable_list_add(hittable_list *list, t_hittable *object);
// bool hittable_list_hit(const hittable_list *list, const t_ray *r, float tmin, float tmax, t_hit *rec);
bool hittable_list_hit(const hittable_list *list, const t_ray *r, float tmin, float tmax, t_hit *rec);

/*Experimental*/

typedef struct s_light
{
    t_tuple position;    // Position of the light in the scene
    t_color intensity;   // Intensity (color) of the light
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
    //t_tuple (*local_normal_at)(const struct s_plane *plane, t_tuple point);
} t_plane;

typedef struct s_cylinder
{
	t_tuple	  	normal;
    t_matrix 	*transform;
    t_matrix 	*inverse_transform;
	t_matrix 	*translation_matrix;
	t_matrix 	*rotation_matrix;
	t_matrix 	*scaling_matrix;
	t_hittable 	base;	// Inherit hittable structure
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
	t_tuple position;
	t_tuple forward;
	t_tuple right;
	t_tuple up;
	float fov; // Field of view in degrees
	float aspect_ratio;
	float viewport_height;
	float viewport_width;
	float focal_length;

	t_tuple horizontal;
	t_tuple vertical;
	t_tuple lower_left_corner;
} t_cam;

typedef struct s_var
{
	mlx_t *mlx;
	t_vec3 *vector;

	float image_width;
	float image_height;

	int screen_width;
	t_vec3 camera_center;
	float fov;			   // FOV in degrees
	float theta;		   // Convert FOV to radians
	 // Height of the viewport at a focal length of 1 unit

	t_vec3 pixel_delta_u;
	t_vec3 pixel_delta_v;
	t_vec3 pixel00_loc;

	t_vec3 light_position; // Test light
	mlx_image_t *testimage;
	//hittable_list hittables;
	t_color ambientl;
	t_cam cam;

	t_sphere 	*test_sphere;
	t_plane	 	*test_plane;
	t_cylinder 	*test_cylinder;
	t_light 	*test_light;
	int 		num_spheres;
	int			num_planes;
	int 		num_cylinders;
	int			num_lights;
} t_var;

void initialize_camera(t_var *var, t_cam *camera, t_map *map);

/*MLX*/
void ft_hook(void *param);
void hooks(t_var *var);
int mlxinit(t_var *var, t_map *map);

void printimage(void *param);

// bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec);

//t_sphere sphere_create(t_vec3 center, float radius, t_color col);
void hittable_init(t_hittable *h, hit_func func);

// t_sphere sphere_create(t_vec3 center, float radius);

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
void write_color(t_color col, t_var *var, int x, int y);
// void write_color(FILE* out, const t_vec3* pixel_color);

/*Vector 3*/
float 	calculate_distance(t_vec3 vec_a, t_vec3 vec_b);
t_vec3 	t_vec3_create(float e0, float e1, float e2);
t_vec3 	t_vec3_multiply_scalar(const t_vec3 *v, float t);
t_vec3 	t_vec3_add_vectors(const t_vec3 *u, const t_vec3 *v);
t_vec3 	t_vec3_divide_scalar(const t_vec3 *v, float t);
t_vec3 	t_vec3_multiply_vectors(const t_vec3 *u, const t_vec3 *v);
t_vec3 	t_vec3_subtract_vectors(const t_vec3 *u, const t_vec3 *v);
float 	t_vec3_magnitude_squared(const t_vec3 *v);
float 	t_vec3_magnitude(const t_vec3 *v);

t_vec3 	*t_vec3_divide(t_vec3 *v, float t);
t_vec3 	*t_vec3_multiply(t_vec3 *v, float t);
t_vec3 	*t_vec3_add(t_vec3 *v, const t_vec3 *u);

float 	t_vec3_dot(const t_vec3 *u, const t_vec3 *v);
t_vec3 	t_vec3_cross(const t_vec3 *u, const t_vec3 *v);
t_vec3 	t_vec3_unit_vector(const t_vec3 *v);

t_vec3 	reflect_vector(t_vec3 v, t_vec3 n);
t_vec3 	normalize_vector(t_vec3 v);

t_vec3 	t_vec3_negate(const t_vec3 *vec);

void	t_vec3_print(const t_vec3 *vec);

/* t_ray */

t_ray ray_create(const t_vec3 *origin, const t_vec3 *direction);
t_vec3 ray_origin(const t_ray *r);
t_vec3 ray_direction(const t_ray *r);
t_vec3 ray_at(const t_ray *r, float t);

/*Sphere*/
bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec);
bool cylinder_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec);
// bool sphere_hit(const t_hittable *self, const t_ray *ray, t_hit *rec, t_vec3 *intersection_point);

// float hit_sphere(const t_vec3 *center, float radius, const t_ray *r);

// bool hit_sphere2(const t_vec3 center, float radius, const t_ray r);

/*t_color*/

t_color t_color_create(float r, float b, float g);
t_color color_multiply_scalar(t_color c, float s);
t_color color_add(t_color c1, t_color c2);
t_color clamp_and_normalize_color(t_color color);

bool ray_intersects_sphere(t_ray ray, t_sphere sphere, float *t);

t_vec3 calculate_intersection_point(t_ray ray, float t);

/*Below is all the test_functions.h definitions*/

t_ray         ray(t_tuple origin, t_tuple direction);
t_tuple       position(t_ray r, double t);
t_tuple       apply_transformation(t_matrix *transformation, t_tuple *point);
t_tuple       matrix_to_tuple(t_matrix *m);
t_matrix    *tuple_to_matrix(t_tuple *t);
t_matrix    *shearing(float xy, float xz, float yx, float yz, float zx, float zy);
t_matrix    *rotation_z(float radians);
t_matrix    *rotation_x(float radians);
t_matrix    *rotation_y(float radians);
t_matrix    *scaling(float x, float y, float z);
t_matrix    *inverse_scaling(float x, float y, float z);
t_matrix    *reflective_scaling(float x, float y, float z);
t_matrix    *submatrix(const t_matrix *m, int remove_row, int remove_col);
float       minor(const t_matrix *m, int row, int col);
float       determinant(const t_matrix *m);
float       determinant_2x2(const t_matrix *m);
float       determinant_3x3(const t_matrix *m);
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
t_matrix    *create_4x4_matrix(float a, float b, float c, float d,
                           float e, float f, float g, float h,
                           float i, float j, float k, float l,
                           float m, float n, float o, float p);
float       t_matrix_get(t_matrix *m, int row, int col);
bool        equal(double a, double b);
t_tuple       tuple(double x, double y, double z, double w);
//t_color       hadamard_product(t_color c1, t_color c2);
bool        compare_colors(t_color c1, t_color c2);
t_color       add_colors(t_color c1, t_color c2);
t_color       subtract_colors(t_color c1, t_color c2);
t_color       multiply_color_scalar(t_color c, double scalar);
t_color       multiply_colors(t_color c1, t_color c2);
void        print_color(t_color c);
t_tuple       point(double x, double y, double z);
t_tuple       vector(double x, double y, double z);
bool        is_point(t_tuple t);
bool        is_vector(t_tuple t);
bool        tuple_equal(t_tuple t1, t_tuple t2);
t_tuple       tuple_add(t_tuple t1, t_tuple t2);
t_tuple       tuple_subtract(t_tuple t1, t_tuple t2);
t_tuple       subtract_vector_from_point(t_tuple point, t_tuple vector);
t_tuple       negate_vector(t_tuple v);
t_tuple       negate_tuple(t_tuple t);
t_tuple       tuple_multiply(t_tuple t, double scalar);
t_tuple       tuple_divide(t_tuple t, double scalar);
double      magnitude(t_tuple v);
bool        magnitude_equal(t_tuple v, double expected_magnitude);
t_tuple       normalize(t_tuple v);
double      dot(t_tuple a, t_tuple b);
t_tuple       cross(t_tuple a, t_tuple b);
float       cofactor(const t_matrix *m, int row, int col);
bool        is_invertible(t_matrix *m);

#endif
