#include "../includes/minirt.h"

t_ray ray_create(const t_vec3 *origin, const t_vec3 *direction)
{
    t_ray r;
    r.orig = *origin;
    r.dir = *direction;
    return r;
}

t_vec3 ray_origin(const t_ray *r)
{
    return r->orig;
}

t_vec3 ray_direction(const t_ray *r)
{
    return r->dir;
}

t_vec3 ray_at(const t_ray *r, double t)
{
    t_vec3 scaled_dir = t_vec3_multiply_scalar(&r->dir, t);
    return t_vec3_add_vectors(&r->orig, &scaled_dir);
}
