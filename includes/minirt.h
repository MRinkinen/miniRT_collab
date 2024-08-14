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

#define SCREEN_HEIGHT 400 // Not in use
#define SCREEN_WIDHT 1500
#define MAX_OBJECTS 100

// Constants
// #define INFINITY DBL_MAX
#define PI 3.1415926535897932385

// Utility Functions
static inline float degrees_to_radians(float degrees) // Ei varmaan saa jäädä tänne!!
{
	return degrees * PI / 180.0;
}
typedef struct s_color
{
	float e[3];
} t_color;

typedef struct s_vec
{
	float e[3];
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
bool hittable_list_hit(const hittable_list *list, const t_ray *r, float tmin, float tmax, t_hit *rec);

typedef struct
{
	t_hittable base;
	t_vec3 center;
	float radius;
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
	mlx_image_t *testimage;

} t_var;

bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec);

t_sphere sphere_create(t_vec3 center, float radius);
void hittable_init(t_hittable *h, hit_func func);
bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec);

t_sphere sphere_create(t_vec3 center, float radius);

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
void write_color(t_vec3 vec, t_var *var, int x, int y);
// void write_color(FILE* out, const t_vec3* pixel_color);

/*Vector 3*/
t_vec3 t_vec3_create(float e0, float e1, float e2);
t_vec3 t_vec3_multiply_scalar(const t_vec3 *v, float t);
t_vec3 t_vec3_add_vectors(const t_vec3 *u, const t_vec3 *v);
t_vec3 t_vec3_divide_scalar(const t_vec3 *v, float t);
t_vec3 t_vec3_multiply_vectors(const t_vec3 *u, const t_vec3 *v);
t_vec3 t_vec3_subtract_vectors(const t_vec3 *u, const t_vec3 *v);
float t_vec3_length_squared(const t_vec3 *v);
float t_vec3_length(const t_vec3 *v);

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
float hit_sphere(const t_vec3 *center, float radius, const t_ray *r);

bool hit_sphere2(const t_vec3 center, float radius, const t_ray r);

#endif
