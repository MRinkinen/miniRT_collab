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

#define SCREEN_HEIGHT 400 // Not in use
#define SCREEN_WIDHT 1500
#define MAX_OBJECTS 100

typedef struct s_color
{
	float r;
	float b;
	float g;
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
typedef struct
{
    t_hittable base;   // Inherit hittable structure
    t_vec3 center;     // Center of the base of the cylinder
    t_vec3 orientation; // Orientation of the cylinder (usually represented by a vector)
    float radius;      // Radius of the cylinder
    float height;      // Height of the cylinder
    t_color color;     // Color of the cylinder
} t_cylinders;

typedef struct
{
	t_hittable base;
	t_vec3 center;
	float radius;
	t_color color;
} t_sphere;

typedef struct s_var
{
	mlx_t *mlx;
	t_vec3 *vector;

	float aspect_ratio;
	int image_width;
	int image_height;

	int screen_width;

	float camrerax;
	float camreray;
	float camreraz;
	t_vec3 camera_center;

	t_vec3 pixel_delta_u;
	t_vec3 pixel_delta_v;
	t_vec3 pixel00_loc;
	mlx_image_t *testimage;
	hittable_list hittables;

} t_var;

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
float calculate_distance(t_vec3 vec_a, t_vec3 vec_b);
t_vec3 t_vec3_create(float e0, float e1, float e2);
t_vec3 t_vec3_multiply_scalar(const t_vec3 *v, float t);
t_vec3 t_vec3_add_vectors(const t_vec3 *u, const t_vec3 *v);
t_vec3 t_vec3_divide_scalar(const t_vec3 *v, float t);
t_vec3 t_vec3_multiply_vectors(const t_vec3 *u, const t_vec3 *v);
t_vec3 t_vec3_subtract_vectors(const t_vec3 *u, const t_vec3 *v);
float t_vec3_magnitude_squared(const t_vec3 *v);
float t_vec3_magnitude(const t_vec3 *v);

t_vec3 *t_vec3_divide(t_vec3 *v, float t);
t_vec3 *t_vec3_multiply(t_vec3 *v, float t);
t_vec3 *t_vec3_add(t_vec3 *v, const t_vec3 *u);

float t_vec3_dot(const t_vec3 *u, const t_vec3 *v);
t_vec3 t_vec3_cross(const t_vec3 *u, const t_vec3 *v);
t_vec3 t_vec3_unit_vector(const t_vec3 *v);

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

t_color t_color_create(float x, float y, float z);
t_color color_multiply_scalar(t_color c, float s);
t_color color_add(t_color c1, t_color c2);

bool ray_intersects_sphere(t_ray ray, t_sphere sphere, float *t);

t_vec3 calculate_intersection_point(t_ray ray, float t);

#endif
