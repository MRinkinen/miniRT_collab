#include "../includes/minirt.h"
#include "../includes/parsing.h"


void ray_mul(t_tuple *dest, const t_ray *ray, float t)
{
    dest->x = ray->origin.x + ray->direction.x * t;
    dest->y = ray->origin.y + ray->direction.y * t;
    dest->z = ray->origin.z + ray->direction.z * t;
    dest->w = 1;
}

bool intersect_plane(const t_ray *ray, const t_plane *plane, t_hit *hit)
{
    float	denom;
	t_tuple	tmp;

	denom = dot(plane->orientation, ray->direction);
	if (denom == 0)
		return (false);
	tmp = subtract_vector_from_point(ray->origin, plane->center);
	hit->t = dot(tmp, plane->orientation) / denom;
	if (hit->t < EPSILON)
		return (false);
	ray_mul(&hit->point, ray, hit->t);
	hit->normal = plane->orientation;
	if (dot(hit->normal, ray->direction) > 0)
		hit->normal = tuple_negate(hit->normal);
	return (true);
}


