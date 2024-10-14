#include "../includes/minirt.h"
#include "../includes/parsing.h"

t_sphere sphere_create(t_tuple center, float radius, t_color col)
{
    //printf("Creating sphere at (%f, %f, %f) with radius %f\n", center.x, center.y, center.z, radius);
    t_sphere sphere;

    //hittable_init(&sphere.base, sphere_hit);
    sphere.color = col;
    sphere.center = center;
    sphere.radius = fmax(0, radius);

     // Initialize transformation matrices
    sphere.translation_matrix = translation(center.x, center.y, center.z);
    sphere.rotation_matrix = identity_matrix(); // No rotation initially
    sphere.scaling_matrix = scaling(radius, radius, radius);
    //printf("CHECK THIIIS!!!!\n");
    //printf("Sphere Center Z: %f\n", center.z);
    //printf("Translation Matrix Z-component: %f\n", sphere.translation_matrix->data[2][3]);

    // Combine transformations into one matrix
    sphere.transform = t_matrix_multiply(t_matrix_multiply(sphere.translation_matrix, sphere.rotation_matrix), sphere.scaling_matrix);

    // Calculate the inverse transform for ray-sphere intersection
    sphere.inverse_transform = inverse(sphere.transform);
    return (sphere);
}

t_tuple calculate_sphere_normal(const t_sphere *sphere, const t_tuple *point)
{
    return normalize(tuple_subtract(*point, sphere->center));
}

bool intersect_sphere(const t_ray *ray, const t_sphere *sphere, float *t)
{
    t_tuple oc = tuple_subtract(ray->origin, sphere->center);
    float a = dot(ray->direction, ray->direction);
    float b = 2.0f * dot(oc, ray->direction);
    float c = dot(oc, oc) - sphere->radius * sphere->radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    } else {
        *t = (-b - sqrt(discriminant)) / (2.0f * a);
        return true;
    }
}

