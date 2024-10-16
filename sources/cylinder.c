#include "../includes/minirt.h"
#include "../includes/parsing.h"

// t_cylinder cylinder_create(t_tuple center, float radius, float height, t_color color, t_tuple orientation)
// {
//     t_cylinder cylinder;

//     // Initialize transformation matrices
//     cylinder.translation_matrix = translation(center.x, center.y, center.z);
//     cylinder.rotation_matrix = rotation_from_normal(orientation); // Adjust orientation based on the given vector
//     cylinder.scaling_matrix = scaling(radius, 1.0f, radius); // Scale by radius in xz and height in y

//     // Combine transformations into one matrix
//     cylinder.transform = t_matrix_multiply(t_matrix_multiply(cylinder.translation_matrix, cylinder.rotation_matrix), cylinder.scaling_matrix);

//     // Calculate the inverse transform for ray-cylinder intersection calculations
//     cylinder.inverse_transform = inverse(cylinder.transform);

//     // Set other cylinder properties
//     cylinder.color = color;
//     cylinder.radius = radius;
//     cylinder.height = height;
//     cylinder.normal = orientation;

//     //cylinder.minimum = -INFINITY;
//     //cylinder.maximum = INFINITY;
//     cylinder.minimum = -height / 2;
//     cylinder.maximum = height / 2;

//     return (cylinder);
// }

// Function to calculate the normal vector at a point on the cylinder's surface
t_tuple calculate_cylinder_normal(const t_cylinder *cylinder, const t_tuple *point)
{
    t_tuple center_to_point = tuple_subtract(*point, cylinder->center);
    t_tuple normal = tuple(center_to_point.x, 0.0f, center_to_point.z,0); // Project onto xz-plane
    return normalize(normal);
}

bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    // Cylinder's axis is aligned with the y-axis
    t_tuple oc = tuple_subtract(ray->origin, cylinder->center);
    t_tuple direction = ray->direction;
    t_tuple axis = {0, 1, 0, 1}; // y-axis Kumpi pitaa olla?? 1 vai 0 !!!!!!!!!!!!

    float a = dot(direction, direction) - pow(dot(direction, axis), 2);
    float b = 2.0 * (dot(direction, oc) - dot(direction, axis) * dot(oc, axis));
    float c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    } else {
        float t0 = (-b - sqrt(discriminant)) / (2.0 * a);
        float t1 = (-b + sqrt(discriminant)) / (2.0 * a);

        // Ensure t0 is the smaller value
        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }

        // Check if the intersection points are within the height of the cylinder
        float half_height = cylinder->height / 2.0;
        float y0 = ray->origin.y + t0 * ray->direction.y;
        float y1 = ray->origin.y + t1 * ray->direction.y;

        if (y0 < cylinder->center.y - half_height || y0 > cylinder->center.y + half_height) {
            if (y1 < cylinder->center.y - half_height || y1 > cylinder->center.y + half_height) {
                return false;
            } else {
                *t = t1;
                return (*t >= 0);
            }
        } else {
            *t = t0;
            return (*t >= 0);
        }
    }
}
