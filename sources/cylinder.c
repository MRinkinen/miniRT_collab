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

bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    t_ray transformed_ray;
    transformed_ray.origin = apply_transformation(cylinder->inverse_transform, &ray->origin);
    transformed_ray.direction = apply_transformation(cylinder->inverse_transform, &ray->direction);

    t_tuple oc = tuple_subtract(cylinder->center, ray->origin);
    t_tuple direction = ray->direction;
    t_tuple axis = cylinder->orientation;

    float a = dot(direction, direction) - pow(dot(direction, axis), 2);
    float b = (dot(direction, oc) - dot(direction, axis) * dot(oc, axis));
    float c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;
    float discriminant = (b * b) - (a * c);

    float t0, t1;
    bool hit = false;

    if (discriminant >= 0) {
        t0 = (b - sqrt(discriminant)) / a;
        t1 = (b + sqrt(discriminant)) / a;

        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }

        float half_height = cylinder->height / 2.0;
        t_tuple point0 = tuple_add(ray->origin, tuple_multiply(ray->direction, t0));
        t_tuple point1 = tuple_add(ray->origin, tuple_multiply(ray->direction, t1));

        float dist0 = dot(point0, axis) - dot(cylinder->center, axis);
        float dist1 = dot(point1, axis) - dot(cylinder->center, axis);

        if (dist0 >= -half_height && dist0 <= half_height) {
            *t = t0;
            hit = true;
        } else if (dist1 >= -half_height && dist1 <= half_height) {
            *t = t1;
            hit = true;
        }
    }

    // Check for intersection with the caps
    float t_cap;
    t_tuple cap_center;
    t_tuple cap_point;
    float cap_dist;

    // Top cap
    cap_center = tuple_add(cylinder->center, tuple_multiply(axis, cylinder->height / 2.0));
    t_cap = (dot(cap_center, axis) - dot(ray->origin, axis)) / dot(ray->direction, axis);
    cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
    cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), tuple_subtract(cap_point, cap_center)));

    if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t)) {
        *t = t_cap;
        hit = true;
    }

    // Bottom cap
    cap_center = tuple_subtract(cylinder->center, tuple_multiply(axis, cylinder->height / 2.0));
    t_cap = (dot(cap_center, axis) - dot(ray->origin, axis)) / dot(ray->direction, axis);
    cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
    cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), tuple_subtract(cap_point, cap_center)));

    if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t)) {
        *t = t_cap;
        hit = true;
    }

    return hit;
}
