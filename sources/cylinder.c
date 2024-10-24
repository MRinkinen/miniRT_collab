#include "../includes/minirt.h"
#include "../includes/parsing.h"

void print_matrix(t_matrix *matrix)
{
    if (!matrix) {
        printf("Matrix is NULL\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", matrix->data[i][j]);  // Assuming matrix->values[i][j] is how your matrix stores elements
        }
        printf("\n");
    }
}

// Function to calculate the normal vector at a point on the cylinder's surface
t_tuple calculate_cylinder_normal(const t_cylinder *cylinder, const t_tuple *point)
{
    t_tuple center_to_point = tuple_subtract(*point, cylinder->center);
    t_tuple normal = tuple(center_to_point.x, 0.0f, center_to_point.z,0); // Project onto xz-plane
    return normalize(normal);
}

t_tuple tuple_scale(t_tuple t, double scalar)
{
    return tuple_multiply(t, scalar);
}

t_tuple tuple_negate(t_tuple t)
{
    return tuple_multiply(t, -1.0);
}

// bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, t_hit *hit)
// {
//     t_ray transformed_ray;
//     transformed_ray.origin = apply_transformation(cylinder->inverse_transform, &ray->origin);
//     transformed_ray.direction = apply_transformation(cylinder->inverse_transform, &ray->direction);

//     t_tuple oc = tuple_subtract(cylinder->center, ray->origin);
//     t_tuple direction = ray->direction;
//     t_tuple axis = cylinder->orientation;
//     float half_height = cylinder->height / 2.0;
//     float a = dot(direction, direction) - pow(dot(direction, axis), 2);
//     float b = (dot(direction, oc) - dot(direction, axis) * dot(oc, axis));
//     float c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;
//     float discriminant = (b * b) - (a * c);

//     float t0, t1;
//     bool hit_found = false;

//     if (discriminant >= 0) {
//         t0 = (b - sqrt(discriminant)) / a;
//         t1 = (b + sqrt(discriminant)) / a;

//         if (t0 > t1) {
//             float temp = t0;
//             t0 = t1;
//             t1 = temp;
//         }

//         t_tuple point0 = tuple_add(ray->origin, tuple_multiply(ray->direction, t0));
//         t_tuple point1 = tuple_add(ray->origin, tuple_multiply(ray->direction, t1));

//         float dist0 = dot(point0, axis) - dot(cylinder->center, axis);
//         float dist1 = dot(point1, axis) - dot(cylinder->center, axis);

//         if (dist0 >= -half_height && dist0 <= half_height) {
//             hit->t = t0;
//             hit->point = point0;
//             hit->normal = calculate_cylinder_normal(cylinder, &point0);
//             hit_found = true;
//         } else if (dist1 >= -half_height && dist1 <= half_height) {
//             hit->t = t1;
//             hit->point = point1;
//             hit->normal = calculate_cylinder_normal(cylinder, &point1);
//             hit_found = true;
//         }
//     }

//     // Check for intersection with the caps
//     float t_cap;
//     t_tuple cap_center;
//     t_tuple cap_point;
//     float cap_dist;

//     // Top cap
//     cap_center = tuple_add(cylinder->center, tuple_scale(axis, half_height));
//     t_tuple top_cap_normal = axis;
//     t_cap = dot(tuple_subtract(cap_center, ray->origin), top_cap_normal) / dot(ray->direction, top_cap_normal);
//     cap_point = tuple_add(ray->origin, tuple_scale(ray->direction, t_cap));
//     cap_dist = dot(tuple_subtract(cap_point, cap_center), cap_point);

//     if (cap_dist <= cylinder->radius * cylinder->radius && t_cap > 0 && (!hit_found || t_cap < hit->t)) {
//         hit->t = t_cap;
//         hit->point = cap_point;
//         hit->normal = top_cap_normal;
//         hit_found = true;
//     }

//     // Bottom cap
//     cap_center = tuple_subtract(cylinder->center, tuple_scale(axis, half_height));
//     t_tuple bottom_cap_normal = (axis);
//     t_cap = dot(tuple_subtract(cap_center, ray->origin), bottom_cap_normal) / dot(ray->direction, bottom_cap_normal);
//     cap_point = tuple_add(ray->origin, tuple_scale(ray->direction, t_cap));
//     cap_dist = dot(tuple_subtract(cap_point, cap_center), cap_point);

//     if (cap_dist <= cylinder->radius * cylinder->radius && t_cap > 0 && (!hit_found || t_cap < hit->t)) {
//         hit->t = t_cap;
//         hit->point = cap_point;
//         hit->normal = bottom_cap_normal;
//         hit_found = true;
//     }

//     return hit_found;
// }

bool	solve_quadratic(t_quadratic *q)
{
	q->delta = q->b * q->b - 4 * q->a * q->c;
	if (q->delta < 0)
		return (false);
	q->t1 = (-q->b - sqrt(q->delta)) / (q->a * 2);
	q->t2 = (-q->b + sqrt(q->delta)) / (q->a * 2);
	return (true);
}

bool infinite_cyl_inter(t_ray *r, t_cylinder *cy, t_hit *hit)
{
    t_quadratic q;
    t_tuple u;
    t_tuple v;

    u = cross(r->direction, cy->orientation);
    v = subtract_vector_from_point(cy->center, r->origin);
    v = cross(v, cy->orientation);
    q.a = dot(u, u);
    q.b = 2 * dot(u, v);
    q.c = dot(v, v) - cy->radius_squared;
    if (!solve_quadratic(&q) || (q.t2 <= EPSILON && q.t1 <= EPSILON))
        return (false);
    if (q.t1 <= EPSILON || (q.t2 > EPSILON && (q.t2 < q.t1)))
        q.t1 = q.t2;
    hit->t = q.t1;
    hit->point = tuple_add(r->origin, tuple_multiply(r->direction, q.t1));
    v = subtract_vector_from_point(cy->center, hit->point);
    hit->normal = cross(v, cy->orientation);
    hit->normal = cross(hit->normal, cy->orientation);
    normalize(hit->normal);
    if (dot(hit->normal, r->direction) > 0)
        hit->normal = tuple_negate(hit->normal);
    return (true);
}

bool intersect_cylinder(t_ray *r, t_cylinder *cy, t_hit *hit)
{
    t_plane pl;
    t_hit tmp_hit;

    hit->t = INFINITY;
    pl.center = cy->top_cap_center;
    pl.orientation = cy->orientation;

    // Check intersection with the top cap of the cylinder
    if (intersect_plane(r, &pl, &tmp_hit)
        && distance(tmp_hit.point, cy->top_cap_center) <= cy->radius * 0.5
        && hit->t > tmp_hit.t)
    {
        *hit = tmp_hit;
    }

    // Check intersection with the bottom cap of the cylinder
    pl.center = cy->bottom_cap_center;
    if (intersect_plane(r, &pl, &tmp_hit)
        && distance(tmp_hit.point, cy->bottom_cap_center) <= cy->radius * 0.5
        && hit->t > tmp_hit.t)
    {
        *hit = tmp_hit;
    }

    // Check intersection with the infinite cylinder
    if (infinite_cyl_inter(r, cy, &tmp_hit)
        && pow(distance(cy->center, tmp_hit.point), 2)
        <= pow(cy->height * 0.5, 2) + cy->radius * cy->radius
        && hit->t > tmp_hit.t)
    {
        *hit = tmp_hit;
    }

    return (hit->t < INFINITY && hit->t > EPSILON);
}
