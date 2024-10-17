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

/*
t_cylinder cylinder_create(t_tuple center, float radius, float height, t_color color, t_tuple orientation)
{
    t_cylinder cylinder;

    // Initialize transformation matrices
    cylinder.translation_matrix = translation(center.x, center.y, center.z);
    cylinder.rotation_matrix = rotation_from_normal(orientation); // Adjust orientation based on the given vector
    cylinder.scaling_matrix = scaling(radius, 1.0f, radius); // Scale by radius in xz and height in y

    // Combine transformations into one matrix
    cylinder.transform = t_matrix_multiply(t_matrix_multiply(cylinder.translation_matrix, cylinder.rotation_matrix), cylinder.scaling_matrix);

    // Calculate the inverse transform for ray-cylinder intersection calculations
    cylinder.inverse_transform = inverse(cylinder.transform);

    // Set other cylinder properties
    cylinder.color = color;
    cylinder.radius = radius;
    cylinder.height = height;
    cylinder.normal = orientation;

    //cylinder.minimum = -INFINITY;
    //cylinder.maximum = INFINITY;
    cylinder.minimum = -height / 2;
    cylinder.maximum = height / 2;

    return (cylinder);
}*/

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
    t_tuple axis = cylinder->orientation; // Use the cylinder's actual orientation vector

    float a = dot(direction, direction) - pow(dot(direction, axis), 2);
    float b = (dot(direction, oc) - dot(direction, axis) * dot(oc, axis));
    float c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;
    float discriminant = (b * b) - (a * c);

    if (discriminant < 0) {
        return false;
    } else {
        float t0 = (b - sqrt(discriminant)) / a;
        float t1 = (b + sqrt(discriminant)) / a;

        // Ensure t0 is the smaller value
        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        // Check if the intersection points are within the height of the cylinder
        float half_height = cylinder->height / 2.0;

        // Project the points onto the cylinder's axis
        t_tuple point0 = tuple_add(ray->origin, tuple_multiply(ray->direction, t0));
        t_tuple point1 = tuple_add(ray->origin, tuple_multiply(ray->direction, t1));

        float dist0 = dot(point0, axis) - dot(cylinder->center, axis);
        float dist1 = dot(point1, axis) - dot(cylinder->center, axis);

        if (dist0 < -half_height || dist0 > half_height) {
            if (dist1 < -half_height || dist1 > half_height) {
                return false; // Both intersections are outside the capped height
            } else {
                *t = t1; // t1 is within the height
                return (*t >= 0);
            }
        } else {
            *t = t0; // t0 is within the height
            return (*t >= 0);
        }
    }
}
