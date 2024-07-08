#include "../includes/minirt.h"

// using color = t_vec3;

void write_color(t_vec3 vec, t_var *var, int x, int y)
{
    // int color = ft_pixel(vector3->e[0],vector3->e[1],vector3->e[2],255);
    double r = vec.e[0];
    double g = vec.e[1];
    double b = vec.e[2];

    // Translate the [0,1] component values to the byte range [0,255].
    int ir = (int)(255.999 * r);
    int ig = (int)(255.999 * g);
    int ib = (int)(255.999 * b);

    int color = ft_pixel(ir, ig, ib, 255);
    mlx_put_pixel(var->testimage, x, y, color);
}

// Assuming t_color is defined similar to t_vec3 for color representation

// Utility functions for vector operations, assuming these are defined elsewhere
// t_vec3 vec3_subtract(t_vec3 a, t_vec3 b);
// double vec3_dot(t_vec3 a, t_vec3 b);
// t_vec3 vec3_multiply_scalar(t_vec3 v, double s);
// t_vec3 vec3_unit_vector(t_vec3 v);
// t_color color(double r, double g, double b);

// bool hit_sphere2(const t_vec3 center, double radius, const t_ray r)
// {

//     t_vec3 oc = t_vec3_subtract_vectors(&r.orig, &center);
//     double a = t_vec3_dot(&r.dir, &r.dir);
//     double b = 2.0 * t_vec3_dot(&r.dir, &oc);
//     double c = t_vec3_dot(&oc, &oc) - radius * radius;
//     double discriminant = b * b - 4 * a * c;
//     return (discriminant >= 0);
// }

// double hit_sphere(const t_vec3 *center, double radius, const t_ray *r)
// {
//     t_vec3 oc = t_vec3_subtract_vectors(center, &r->orig);
//     double a = t_vec3_dot(&r->dir, &r->dir);
//     double b = -2.0 * t_vec3_dot(&r->dir, &oc);
//     double c = t_vec3_dot(&oc, &oc) - radius * radius;
//     double discriminant = b * b - 4 * a * c;
//     if (discriminant < 0)
//     {
//         return -1.0;
//     }
//     else
//     {
//         return (-b - sqrt(discriminant)) / (2.0 * a);
//     }
// }
void hittable_init(t_hittable *h, hit_func func)
{
    h->hit = func;
}

bool sphere_hit(const t_hittable *self, const t_ray *r, double tmin, double tmax, t_hit *rec)
{
    const t_sphere *s = (const t_sphere *)self;
    t_vec3 oc = t_vec3_subtract_vectors(&r->orig, &s->center);
    double a = t_vec3_dot(&r->dir, &r->dir);
    double h = t_vec3_dot(&oc, &r->dir);
    double c = t_vec3_dot(&oc, &oc) - s->radius * s->radius;

    double discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (h - sqrtd) / a;
    if (root < tmin || tmax < root)
    {
        root = (h + sqrtd) / a;
        if (root < tmin || tmax < root)
            return false;
    }

    rec->t = root;
    rec->p = ray_at(r, rec->t);
    t_vec3 temp = t_vec3_subtract_vectors(&rec->p, &s->center);
    rec->normal = t_vec3_multiply_scalar(&temp, 1.0 / s->radius);

    return true;
}

t_sphere sphere_create(t_vec3 center, double radius)
{
    t_sphere s;

    hittable_init(&s.base, sphere_hit);
    s.center = center;
    s.radius = fmax(0, radius);
    return s;
}
