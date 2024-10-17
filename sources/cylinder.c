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
/*
t_quad_coeffs quadratic_coeffs_cylinder(const t_ray *ray, const t_cylinder *cylinder)
{
    t_quad_coeffs coeffs;
    t_tuple oc = tuple_subtract(ray->origin, cylinder->center);
    t_tuple axis = cylinder->orientation;  // Use cylinder's orientation vector

    coeffs.a = dot(ray->direction, ray->direction) - pow(dot(ray->direction, axis), 2);
    coeffs.b = 2.0 * (dot(ray->direction, oc) - dot(ray->direction, axis) * dot(oc, axis));
    coeffs.c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;

    return coeffs;
}

double intersect_cylinder_caps(const t_ray *ray, const t_cylinder *cylinder)
{
    t_tuple cap_center[2];
    double t_caps[2];
    double r_squared;
    t_tuple cap_intersection;
    int i;

    for (i = 0; i < 2; i++)
    {
        // Compute cap center points
        cap_center[i] = tuple_add(cylinder->center, tuple_scale(cylinder->orientation, (i * 2 - 1) * (cylinder->height / 2.0)));
        
        // Intersection with the plane of the caps
        t_caps[i] = dot(tuple_subtract(cap_center[i], ray->origin), cylinder->orientation) / dot(ray->direction, cylinder->orientation);
        
        if (t_caps[i] > 0)
        {
            cap_intersection = tuple_add(ray->origin, tuple_scale(ray->direction, t_caps[i]));
            t_tuple dist_to_cap = tuple_subtract(cap_intersection, cap_center[i]);
            r_squared = dot(dist_to_cap, dist_to_cap);
            
            // If intersection is outside the cap's radius, discard
            if (r_squared > cylinder->radius * cylinder->radius)
                t_caps[i] = INFINITY;
        }
        else
            t_caps[i] = INFINITY;
    }
    
    return fmin(t_caps[0], t_caps[1]);
}

bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    t_quad_coeffs coeffs = quadratic_coeffs_cylinder(ray, cylinder);
    float discriminant = coeffs.b * coeffs.b - 4 * coeffs.a * coeffs.c;

    if (discriminant < 0)
        return false;  // No intersection, return early

    float t_body[2];
    t_body[0] = (-coeffs.b - sqrt(discriminant)) / (2.0 * coeffs.a);
    t_body[1] = (-coeffs.b + sqrt(discriminant)) / (2.0 * coeffs.a);

    // Ensure t_body[0] is the smaller value
    if (t_body[0] > t_body[1]) {
        float temp = t_body[0];
        t_body[0] = t_body[1];
        t_body[1] = temp;
    }

    // Check if the intersection points are within the height of the cylinder
    float half_height = cylinder->height / 2.0;
    t_tuple point_body = tuple_add(ray->origin, tuple_scale(ray->direction, t_body[0]));
    float y_body = dot(tuple_subtract(point_body, cylinder->center), cylinder->orientation);

    if (fabs(y_body) > half_height)
        t_body[0] = INFINITY;

    // Check for intersection with the caps
    float t_caps = intersect_cylinder_caps(ray, cylinder);

    // Return the smallest valid intersection
    *t = fmin(t_body[0], t_caps);

    // Only print information if an intersection occurs
    if (*t != INFINITY) {
        // Print ray data
        printf("Ray Origin: (%f, %f, %f)\n", ray->origin.x, ray->origin.y, ray->origin.z);
        printf("Ray Direction: (%f, %f, %f)\n", ray->direction.x, ray->direction.y, ray->direction.z);

        // Print cylinder data
        printf("Cylinder Center: (%f, %f, %f)\n", cylinder->center.x, cylinder->center.y, cylinder->center.z);
        printf("Cylinder Orientation: (%f, %f, %f)\n", cylinder->orientation.x, cylinder->orientation.y, cylinder->orientation.z);
        printf("Cylinder Radius: %f\n", cylinder->radius);
        printf("Cylinder Height: %f\n", cylinder->height);

        // Print quadratic coefficients
        printf("Quadratic Coefficients:\n");
        printf("a: %f, b: %f, c: %f\n", coeffs.a, coeffs.b, coeffs.c);
        printf("Discriminant: %f\n", discriminant);

        // Print body intersection times
        printf("Body Intersection t0: %f, t1: %f\n", t_body[0], t_body[1]);

        // Print the intersection point
        printf("Intersection Point on Body at t0: (%f, %f, %f)\n", point_body.x, point_body.y, point_body.z);

        // Print y_body (distance along the cylinder's axis)
        printf("y_body (distance along cylinder axis): %f\n", y_body);

        // Print cap intersection time
        printf("Cap Intersection t_caps: %f\n", t_caps);

        // Print final intersection time
        printf("Final Intersection t: %f\n", *t);
    }

    return (*t != INFINITY);
}

t_tuple tuple_scale(t_tuple t, double scalar)
{
    return tuple_multiply(t, scalar);
}

t_quad_coeffs quadratic_coeffs_cylinder(const t_ray *ray, const t_cylinder *cylinder)
{
    t_quad_coeffs coeffs;
    t_tuple oc = tuple_subtract(ray->origin, cylinder->center);
    t_tuple axis = cylinder->orientation;  // Use cylinder's orientation vector

    coeffs.a = dot(ray->direction, ray->direction) - pow(dot(ray->direction, axis), 2);
    coeffs.b = 2.0 * (dot(ray->direction, oc) - dot(ray->direction, axis) * dot(oc, axis));
    coeffs.c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;

    return coeffs;
}

double intersect_cylinder_caps(const t_ray *ray, const t_cylinder *cylinder)
{
    t_tuple cap_center[2];
    double t_caps[2];
    double r_squared;
    t_tuple cap_intersection;
    int i;

    for (i = 0; i < 2; i++)
    {
        // Compute cap center points
        cap_center[i] = tuple_add(cylinder->center, tuple_scale(cylinder->orientation, (i * 2 - 1) * (cylinder->height / 2.0)));
        
        // Intersection with the plane of the caps
        t_caps[i] = dot(tuple_subtract(cap_center[i], ray->origin), cylinder->orientation) / dot(ray->direction, cylinder->orientation);
        
        if (t_caps[i] > 0)
        {
            cap_intersection = tuple_add(ray->origin, tuple_scale(ray->direction, t_caps[i]));
            t_tuple dist_to_cap = tuple_subtract(cap_intersection, cap_center[i]);
            r_squared = dot(dist_to_cap, dist_to_cap);
            
            // If intersection is outside the cap's radius, discard
            if (r_squared > cylinder->radius * cylinder->radius)
                t_caps[i] = INFINITY;
        }
        else
            t_caps[i] = INFINITY;
    }
    
    return fmin(t_caps[0], t_caps[1]);
}
*/
/*
bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    t_quad_coeffs coeffs = quadratic_coeffs_cylinder(ray, cylinder);
    float discriminant = coeffs.b * coeffs.b - 4 * coeffs.a * coeffs.c;

    if (discriminant < 0)
        return false;  // No intersection, return early

    float t_body[2];
    t_body[0] = (-coeffs.b - sqrt(discriminant)) / (2.0 * coeffs.a);
    t_body[1] = (-coeffs.b + sqrt(discriminant)) / (2.0 * coeffs.a);

    // Ensure t_body[0] is the smaller value
    if (t_body[0] > t_body[1]) {
        float temp = t_body[0];
        t_body[0] = t_body[1];
        t_body[1] = temp;
    }

    // Check if the intersection points are within the height of the cylinder
    float half_height = cylinder->height / 2.0;
    t_tuple point_body = tuple_add(ray->origin, tuple_scale(ray->direction, t_body[0]));
    float y_body = dot(tuple_subtract(point_body, cylinder->center), cylinder->orientation);

    if (fabs(y_body) > half_height)
        t_body[0] = INFINITY;

    // Check for intersection with the caps
    float t_caps = intersect_cylinder_caps(ray, cylinder);

    // Return the smallest valid intersection
    *t = fmin(t_body[0], t_caps);

    // Only print information if an intersection occurs
    if (*t != INFINITY) {
        // Print ray data
        printf("Ray Origin: (%f, %f, %f)\n", ray->origin.x, ray->origin.y, ray->origin.z);
        printf("Ray Direction: (%f, %f, %f)\n", ray->direction.x, ray->direction.y, ray->direction.z);

        // Print cylinder data
        printf("Cylinder Center: (%f, %f, %f)\n", cylinder->center.x, cylinder->center.y, cylinder->center.z);
        printf("Cylinder Orientation: (%f, %f, %f)\n", cylinder->orientation.x, cylinder->orientation.y, cylinder->orientation.z);
        printf("Cylinder Radius: %f\n", cylinder->radius);
        printf("Cylinder Height: %f\n", cylinder->height);

        // Print quadratic coefficients
        printf("Quadratic Coefficients:\n");
        printf("a: %f, b: %f, c: %f\n", coeffs.a, coeffs.b, coeffs.c);
        printf("Discriminant: %f\n", discriminant);

        // Print body intersection times
        printf("Body Intersection t0: %f, t1: %f\n", t_body[0], t_body[1]);

        // Print the intersection point
        printf("Intersection Point on Body at t0: (%f, %f, %f)\n", point_body.x, point_body.y, point_body.z);

        // Print y_body (distance along the cylinder's axis)
        printf("y_body (distance along cylinder axis): %f\n", y_body);

        // Print cap intersection time
        printf("Cap Intersection t_caps: %f\n", t_caps);

        // Print final intersection time
        printf("Final Intersection t: %f\n", *t);
    }

    return (*t != INFINITY);
}*/


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
        t_tuple point0 = tuple_add(ray->origin, tuple_scale(ray->direction, t0));
        t_tuple point1 = tuple_add(ray->origin, tuple_scale(ray->direction, t1));

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

/*
bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    // Transform the ray using the inverse of the cylinder's transformation matrix
    t_ray transformed_ray;
    transformed_ray.origin = apply_transformation(cylinder->inverse_transform, &ray->origin);
    transformed_ray.direction = apply_transformation(cylinder->inverse_transform, &ray->direction);

    // Compute oc and the axis using the transformed ray and cylinder properties
    t_tuple oc = tuple_subtract(transformed_ray.origin, cylinder->center);
    t_tuple direction = transformed_ray.direction;
    t_tuple axis = cylinder->normal; // Use the cylinder's actual orientation vector

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
        float y0 = transformed_ray.origin.y + t0 * transformed_ray.direction.y;
        float y1 = transformed_ray.origin.y + t1 * transformed_ray.direction.y;

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
}*/

/*
bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    t_ray transformed_ray;
    transformed_ray.origin = apply_transformation(cylinder->inverse_transform, &ray->origin);
    transformed_ray.direction = apply_transformation(cylinder->inverse_transform, &ray->direction);

    // Cylinder's axis is aligned with the y-axis
    t_tuple oc = tuple_subtract(ray->origin, cylinder->center);
    t_tuple direction = transformed_ray.direction;
    t_tuple axis = cylinder->orientation;
    //t_tuple axis = cylinder->normal;

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
}*/
