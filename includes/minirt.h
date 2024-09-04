#ifndef MINIRT_H
#define MINIRT_H

#include <stdbool.h>
#include <stdio.h> // DELETE BEFORE RETURN!!
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include "../libft/libft.h"
#include "../MLX42/include/MLX42/MLX42.h"
#include "./parsing.h"
//#include "./test_functions.h"

#define SCREEN_HEIGHT 600 // Not in use
#define SCREEN_WIDTH 1500
#define MAX_OBJECTS 100
#define PI 3.141592653589793
#define EPSILON 0.00001

typedef struct
{
    int rows;
    int cols;
    float data[4][4]; // This can hold up to a 4x4 matrix, adjust the size if necessary
} t_matrix;

typedef struct
{
    double red;
    double green;
    double blue;
} Color;

/* Above is all test_functions.h definitions*/

typedef struct
{
    double x, y, z, w;
}         Tuple;

typedef struct 
{
    Tuple origin;
    Tuple direction;
} Ray;

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

typedef struct s_ray
{
	t_vec3 orig;
	t_vec3 dir;
} t_ray;

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
typedef struct s_cylinders
{
	t_hittable base;	// Inherit hittable structure
	t_vec3 center;		// Center of the base of the cylinder
	t_vec3 orientation; // Orientation of the cylinder (usually represented by a vector)
	float radius;		// Radius of the cylinder
	float height;		// Height of the cylinder
	t_color color;		// Color of the cylinder
} t_cylinders;

typedef struct s_sphere
{
	t_hittable base;
	t_vec3 center;
	float radius;
	t_color color;
} t_sphere;

typedef struct s_ambienlight
{
	t_color color;
	float intensity;
} t_ambienlight;

typedef struct
{
	t_vec3 position;
	t_vec3 forward;
	t_vec3 right;
	t_vec3 up;
	float fov; // Field of view in degrees
	float aspect_ratio;
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
	float viewport_height; // Height of the viewport at a focal length of 1 unit
	float viewport_width;
	t_vec3 pixel_delta_u;
	t_vec3 pixel_delta_v;
	t_vec3 pixel00_loc;

	t_vec3 light_position; // Test light
	mlx_image_t *testimage;
	hittable_list hittables;
	t_ambienlight ambientl;
	t_cam cam;
} t_var;



void initialize_camera(t_cam *camera, t_vec3 position, t_vec3 look_at, t_vec3 up, float fov, float aspect_ratio);

/*MLX*/
void ft_hook(void *param);
void hooks(t_var *var);
int mlxinit(t_var *var, t_map *map);

void printimage(void *param, t_map *map);

// bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec);

t_sphere sphere_create(t_vec3 center, float radius, t_color col);
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

/* Ray */

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

/*Color*/

t_color t_color_create(float r, float b, float g);
t_color color_multiply_scalar(t_color c, float s);
t_color color_add(t_color c1, t_color c2);
t_color clamp_and_normalize_color(t_color color);

bool ray_intersects_sphere(t_ray ray, t_sphere sphere, float *t);

t_vec3 calculate_intersection_point(t_ray ray, float t);

/*Below is all the test_functions.h definitions*/

Ray         ray(Tuple origin, Tuple direction);
Tuple       position(Ray r, double t);
Tuple       apply_transformation(t_matrix *transformation, Tuple *point);
Tuple       matrix_to_tuple(t_matrix *m);
t_matrix    *tuple_to_matrix(Tuple *t);
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
Tuple       tuple(double x, double y, double z, double w);
//Color       hadamard_product(Color c1, Color c2);
bool        compare_colors(Color c1, Color c2);
Color       add_colors(Color c1, Color c2);
Color       subtract_colors(Color c1, Color c2);
Color       multiply_color_scalar(Color c, double scalar);
Color       multiply_colors(Color c1, Color c2);
void        print_color(Color c);
Tuple       point(double x, double y, double z);
Tuple       vector(double x, double y, double z);
bool        is_point(Tuple t);
bool        is_vector(Tuple t);
bool        tuple_equal(Tuple t1, Tuple t2);
Tuple       tuple_add(Tuple t1, Tuple t2);
Tuple       tuple_subtract(Tuple t1, Tuple t2);
Tuple       subtract_vector_from_point(Tuple point, Tuple vector);
Tuple       negate_vector(Tuple v);
Tuple       negate_tuple(Tuple t);
Tuple       tuple_multiply(Tuple t, double scalar);
Tuple       tuple_divide(Tuple t, double scalar);
double      magnitude(Tuple v);
bool        magnitude_equal(Tuple v, double expected_magnitude);
Tuple       normalize(Tuple v);
double      dot(Tuple a, Tuple b);
Tuple       cross(Tuple a, Tuple b);
float       cofactor(const t_matrix *m, int row, int col);
bool        is_invertible(t_matrix *m);

#endif
