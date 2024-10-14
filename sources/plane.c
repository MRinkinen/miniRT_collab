#include "../includes/minirt.h"
#include "../includes/parsing.h"

t_plane plane_create(t_tuple center, t_color color, t_tuple orientation)
{
    t_plane plane;

    printf("plane normals: x: %f, y: %f, z: %f, w: %f\n", orientation.x, orientation.y, orientation.z, orientation.w);
    // Initialize transformation matrices
    plane.translation_matrix = translation(center.x, center.y, center.z);
    plane.rotation_matrix = rotation_from_normal(orientation); // Adjust orientation
    //plane.rotation_matrix = identity_matrix();
    plane.scaling_matrix = scaling((float)1, (float)1, (float)1);

    // Combine transformations into one matrix
    //plane.transform = t_matrix_multiply(plane.translation_matrix, t_matrix_multiply(plane.rotation_matrix, plane.scaling_matrix));
    plane.transform = t_matrix_multiply(t_matrix_multiply(plane.translation_matrix, plane.rotation_matrix), plane.scaling_matrix);

    // Calculate the inverse transform for ray-plane intersection calculations
    plane.inverse_transform = inverse(plane.transform);
    plane.color = color;
    plane.normal = orientation;

    return (plane);
}

bool intersect_plane(const t_ray *ray, const t_plane *plane, float *t)
{
    float denom = dot(plane->normal, ray->direction);
    if (fabs(denom) > 1e-6) { // Check if the ray is not parallel to the plane
        t_tuple p0l0 = tuple_subtract(plane->point, ray->origin);
        *t = dot(p0l0, plane->normal) / denom;
        return (*t >= 0);
    }
    return false;
}
