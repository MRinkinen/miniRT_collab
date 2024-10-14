/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/14 12:36:36 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"
//#include "../includes/test_functions.h"


t_tuple reflect(t_tuple in, t_tuple normal)
{
    // Calculate dot product of 'in' and 'normal'
    double dot_product = dot(in, normal);
    // Multiply normal vector by 2 * dot_product
    t_tuple scaled_normal = tuple_multiply(normal, 2 * dot_product);
    // Subtract scaled normal from the incoming vector
    return (tuple_subtract(in, scaled_normal));
}

// Take care of orientation vector as well, check that the orientations work
// and dont accept 0,0,0 as direction

// Still need to take care for mallocs in functions like for example:
// t_matrix *scaling();
// t_matrix *inverse();
// t_matrix *cofactor_matrix();
// t_matrix *submatrix();
// t_matrix *identity_matrix();
// t_matrix *transpose();
// t_matrix *t_matrix_multiply();
// t_matrix *create_2x2_matrix();
// t_matrix *create_3x3_matrix();
// t_matrix *create_4x4_matrix();

// On page 188

t_tuple local_normal_at_cylinder(t_cylinder *cyl, t_tuple point)
{
    // Transform the point into the cylinder's local space
    t_tuple object_point = apply_transformation(cyl->inverse_transform, &point);

    float dist = object_point.x * object_point.x + object_point.z * object_point.z;

    t_tuple local_normal;

    if (dist < 1.0f + EPSILON)
    {
        if (object_point.y >= cyl->maximum - EPSILON)
        {
            // Top cap
            local_normal = vector(0, 1, 0);
        }
        else if (object_point.y <= cyl->minimum + EPSILON)
        {
            // Bottom cap
            local_normal = vector(0, -1, 0);
        }
        else
        {
            // Side normal
            local_normal = vector(object_point.x, 0, object_point.z);
        }
    }
    else
    {
        // Side normal
        local_normal = vector(object_point.x, 0, object_point.z);
    }

    // Transform the normal back to world space using inverse transpose
    t_matrix *inverse_transpose = transpose(cyl->inverse_transform);
    t_tuple world_normal = apply_transformation(inverse_transpose, &local_normal);
    free(inverse_transpose); // Free if dynamically allocated

    // Normalize the world normal
    return normalize(world_normal);
}

t_tuple local_normal_at(const t_plane *plane, t_tuple point)
{
    // Log or otherwise use the point to demonstrate it's part of the computation
    // For a plane, the point does not alter the normal, but we consider it to ensure API consistency
    printf("Calculating normal at point: (%f, %f, %f)\n", point.x, point.y, point.z);

    // If transformations are applied to the plane, adjust the normal vector accordingly.
    t_tuple local_normal = vector(0, 1, 0); // Default normal for a plane along XZ.
    t_tuple world_normal = apply_transformation(transpose(inverse(plane->transform)), &local_normal);
    return normalize(world_normal);
}

bool check_cap(t_ray ray, float t)
{
    float x = ray.origin.x + t * ray.direction.x;
    float z = ray.origin.z + t * ray.direction.z;
    return (x * x + z * z) <= 1.0f; // Assuming radius is 1 in local space
}

// The transformed rays origin z value somehow ends up being -20
int cylinder_intersect(t_cylinder cyl, t_ray r, float *t0, float *t1)
{
    //printf("Intersecting ray origin: (%f, %f, %f)\n", r.origin.x, r.origin.y, r.origin.z);
    // Transform ray into the cylinder's local space using the cylinder's inverse transform
    t_tuple transformed_origin = apply_transformation(cyl.inverse_transform, &r.origin);
    t_tuple transformed_direction = apply_transformation(cyl.inverse_transform, &r.direction);
    t_ray transformed_ray = ray(transformed_origin, transformed_direction);
    //printf("Transformed ray origin: (%f, %f, %f)\n", transformed_ray.origin.x, transformed_ray.origin.y, transformed_ray.origin.z);

    // Initialize the number of intersections found
    int intersections = 0;

    // ---- Intersect with the cylinder's side ----
    float a = transformed_ray.direction.x * transformed_ray.direction.x +
              transformed_ray.direction.z * transformed_ray.direction.z;

    // Ray is parallel to the cylinder's y-axis
    if (fabs(a) >= EPSILON)
    {
        float b = 2 * (transformed_ray.origin.x * transformed_ray.direction.x +
                       transformed_ray.origin.z * transformed_ray.direction.z);
        float c = transformed_ray.origin.x * transformed_ray.origin.x +
                  transformed_ray.origin.z * transformed_ray.origin.z - 1; // Assuming radius is 1

        // Calculate the discriminant
        float discriminant = b * b - 4 * a * c;

        // If the discriminant is non-negative, the ray may intersect the cylinder
        if (discriminant >= 0)
        {
            // Calculate the two intersection points using the quadratic formula
            float sqrt_discriminant = sqrt(discriminant);
            float temp_t0 = (-b - sqrt_discriminant) / (2 * a);
            float temp_t1 = (-b + sqrt_discriminant) / (2 * a);

            // Ensure that temp_t0 is the smaller value and temp_t1 is the larger value
            if (temp_t0 > temp_t1)
            {
                float temp = temp_t0;
                temp_t0 = temp_t1;
                temp_t1 = temp;
            }
            // Check the first intersection point
            float y0 = transformed_ray.origin.y + temp_t0 * transformed_ray.direction.y;
            if (cyl.minimum < y0 && y0 < cyl.maximum)
            {
                *t0 = temp_t0;
                intersections++;
            }
            // Check the second intersection point
            float y1 = transformed_ray.origin.y + temp_t1 * transformed_ray.direction.y;
            if (cyl.minimum < y1 && y1 < cyl.maximum)
            {
                if (intersections == 0)
                {
                    *t0 = temp_t1;
                }
                else
                {
                    *t1 = temp_t1;
                }
                intersections++;
            }
        }
    }
    // ---- Intersect with the caps ----
    if (fabs(transformed_ray.direction.y) >= EPSILON)
    {
        // Check for intersection with the lower end cap (y = cyl.minimum)
        float t_cap = (cyl.minimum - transformed_ray.origin.y) / transformed_ray.direction.y;
        if (t_cap >= 0 && check_cap(transformed_ray, t_cap))
        {
            if (intersections == 0)
            {
                *t0 = t_cap;
            }
            else if (intersections == 1)
            {
                *t1 = t_cap;
            }
            intersections++;
        }

        // Check for intersection with the upper end cap (y = cyl.maximum)
        t_cap = (cyl.maximum - transformed_ray.origin.y) / transformed_ray.direction.y;
        if (t_cap >= 0 && check_cap(transformed_ray, t_cap))
        {
            if (intersections == 0)
            {
                *t0 = t_cap;
            }
            else if (intersections == 1)
            {
                *t1 = t_cap;
            }
            intersections++;
        }
    }
    // Validate the Z coordinates of the intersection points
    if (intersections > 0)
    {
        // Validate Z coordinates for both t0 and t1
        if (intersections == 1)
        {
            // If only t0 is set, check its Z coordinate
            float z0 = transformed_ray.origin.z + *t0 * transformed_ray.direction.z;
            if (z0 < 0) // Adjust this condition based on the cylinder's orientation
            {
                intersections = 0; // Discard t0 if it's behind the ray's origin
            }
        }
        else if (intersections == 2)
        {
            float z0 = transformed_ray.origin.z + *t0 * transformed_ray.direction.z;
            float z1 = transformed_ray.origin.z + *t1 * transformed_ray.direction.z;

            // Check both Z coordinates
            if (z0 < 0) // Adjust based on the cylinder's orientation
            {
                *t0 = *t1; // Discard t0 if it's behind the ray's origin
                intersections = 1;
            }
            if (z1 < 0 && z0 >= 0)
            {
                intersections = 1; // If only t1 is behind, discard it
            }
        }
    }
    // // Only print output if there were valid intersections
    // if (intersections > 0)
    // {
    //     printf("Valid intersections found: %d\n", intersections);
    //     if (intersections == 1)
    //     {
    //         printf("t0: %f\n", *t0);
    //     }
    //     else if (intersections == 2)
    //     {
    //         printf("t0: %f, t1: %f\n", *t0, *t1);
    //     }
    // }
    // Only print output if there were valid intersections
    // if (intersections > 0)
    // {
    //     //printf("Valid intersections found: %d\n", intersections);
    //     //printf("Intersecting ray origin: (%f, %f, %f)\n", transformed_ray.origin.x, transformed_ray.origin.y, transformed_ray.origin.z);
    //     //printf("Intersecting ray direction: (%f, %f, %f)\n", transformed_ray.direction.x, transformed_ray.direction.y, transformed_ray.direction.z);

    //     if (intersections == 1)
    //     {
    //         //printf("t0: %f\n", *t0);
    //         // Calculate and print intersection position for t0
    //         float intersection_pos0_x = transformed_ray.origin.x + *t0 * transformed_ray.direction.x;
    //         float intersection_pos0_y = transformed_ray.origin.y + *t0 * transformed_ray.direction.y;
    //         float intersection_pos0_z = transformed_ray.origin.z + *t0 * transformed_ray.direction.z;
    //         //printf("Intersection position for t0: (%f, %f, %f)\n", intersection_pos0_x, intersection_pos0_y, intersection_pos0_z);
    //     }
    //     else if (intersections == 2)
    //     {
    //         //printf("t0: %f, t1: %f\n", *t0, *t1);

    //         // Calculate and print intersection position for t0
    //         float intersection_pos0_x = transformed_ray.origin.x + *t0 * transformed_ray.direction.x;
    //         float intersection_pos0_y = transformed_ray.origin.y + *t0 * transformed_ray.direction.y;
    //         float intersection_pos0_z = transformed_ray.origin.z + *t0 * transformed_ray.direction.z;
    //         //printf("Intersection position for t0: (%f, %f, %f)\n", intersection_pos0_x, intersection_pos0_y, intersection_pos0_z);

    //         // Calculate and print intersection position for t1
    //         float intersection_pos1_x = transformed_ray.origin.x + *t1 * transformed_ray.direction.x;
    //         float intersection_pos1_y = transformed_ray.origin.y + *t1 * transformed_ray.direction.y;
    //         float intersection_pos1_z = transformed_ray.origin.z + *t1 * transformed_ray.direction.z;
    //         //printf("Intersection position for t1: (%f, %f, %f)\n", intersection_pos1_x, intersection_pos1_y, intersection_pos1_z);
    //     }
    // }
    return intersections;
}

/*
int cylinder_intersect(t_cylinder cyl, t_ray r, float *t0, float *t1)
{
    // Transform ray into the cylinder's local space using the cylinder's inverse transform
    t_tuple transformed_origin = apply_transformation(cyl.inverse_transform, &r.origin);
    t_tuple transformed_direction = apply_transformation(cyl.inverse_transform, &r.direction);
    t_ray transformed_ray = ray(transformed_origin, transformed_direction);
    // Initialize the number of intersections found
    int intersections = 0;
    // ---- Intersect with the cylinder's side ----
    float a = transformed_ray.direction.x * transformed_ray.direction.x +
              transformed_ray.direction.z * transformed_ray.direction.z;
    // Ray is parallel to the cylinder's y-axis
    if (fabs(a) >= EPSILON)
    {
        float b = 2 * (transformed_ray.origin.x * transformed_ray.direction.x +
                       transformed_ray.origin.z * transformed_ray.direction.z);
        float c = transformed_ray.origin.x * transformed_ray.origin.x +
                  transformed_ray.origin.z * transformed_ray.origin.z - 1; // Assuming radius is 1

        // Calculate the discriminant
        float discriminant = b * b - 4 * a * c;
        // If the discriminant is negative, the ray does not intersect the cylinder
        if (discriminant >= 0)
        {
            // Calculate the two intersection points using the quadratic formula
            float sqrt_discriminant = sqrt(discriminant);
            float temp_t0 = (-b - sqrt_discriminant) / (2 * a);
            float temp_t1 = (-b + sqrt_discriminant) / (2 * a);
            // Ensure that temp_t0 is the smaller value and temp_t1 is the larger value
            if (temp_t0 > temp_t1)
            {
                float temp = temp_t0;
                temp_t0 = temp_t1;
                temp_t1 = temp;
            }
            // Check the first intersection point
            float y0 = transformed_ray.origin.y + temp_t0 * transformed_ray.direction.y;
            if (cyl.minimum < y0 && y0 < cyl.maximum)
            {
                *t0 = temp_t0;
                intersections++;
            }
            // Check the second intersection point
            float y1 = transformed_ray.origin.y + temp_t1 * transformed_ray.direction.y;
            if (cyl.minimum < y1 && y1 < cyl.maximum)
            {
                if (intersections == 0)
                {
                    *t0 = temp_t1;
                }
                else
                {
                    *t1 = temp_t1;
                }
                intersections++;
            }
        }
    }
    // ---- Intersect with the caps ----
    if (fabs(transformed_ray.direction.y) >= EPSILON)
    {
        // Check for intersection with the lower end cap (y = cyl.minimum)
        float t_cap = (cyl.minimum - transformed_ray.origin.y) / transformed_ray.direction.y;
        if (t_cap >= 0 && check_cap(transformed_ray, t_cap))
        {
            if (intersections == 0)
            {
                *t0 = t_cap;
            }
            else if (intersections == 1)
            {
                *t1 = t_cap;
            }
            intersections++;
        }
        // Check for intersection with the upper end cap (y = cyl.maximum)
        t_cap = (cyl.maximum - transformed_ray.origin.y) / transformed_ray.direction.y;
        if (t_cap >= 0 && check_cap(transformed_ray, t_cap))
        {
            if (intersections == 0)
            {
                *t0 = t_cap;
            }
            else if (intersections == 1)
            {
                *t1 = t_cap;
            }
            intersections++;
        }
    }
    // If the cylinder is positioned in the negative Z direction,
    // we need to check that the Z coordinate of the intersection points is valid
    if (intersections > 0)
    {
        if (intersections == 1)
        {
            // If only t0 is set, check its Z coordinate
            float z0 = transformed_ray.origin.z + *t0 * transformed_ray.direction.z;
            if (z0 < 0) // Adjust this condition based on the cylinder's orientation
            {
                // t0 is behind the ray's origin
                intersections = 0; // No valid intersection
            }
        }
        else if (intersections == 2)
        {
            // Check both t0 and t1
            float z0 = transformed_ray.origin.z + *t0 * transformed_ray.direction.z;
            float z1 = transformed_ray.origin.z + *t1 * transformed_ray.direction.z;

            if (z0 < 0) // Adjust based on the cylinder's orientation
            {
                *t0 = *t1; // Discard t0 if it's behind the ray's origin
                intersections = 1;
            }
            if (z1 < 0 && z0 >= 0)
            {
                // If only t1 is behind, discard it
                intersections = 1;
            }
        }
    }
    return (intersections);
}*/

/*
int cylinder_intersect(t_cylinder cyl, t_ray r, float *t0, float *t1)
{
    // Transform ray into the cylinder's local space using the cylinder's inverse transform
    t_tuple transformed_origin = apply_transformation(cyl.inverse_transform, &r.origin);
    t_tuple transformed_direction = apply_transformation(cyl.inverse_transform, &r.direction);
    t_ray transformed_ray = ray(transformed_origin, transformed_direction);
    // Initialize the number of intersections found
    int intersections = 0;
    // ---- Intersect with the cylinder's side ----
    // Compute the quadratic coefficients
    float a = transformed_ray.direction.x * transformed_ray.direction.x +
              transformed_ray.direction.z * transformed_ray.direction.z;
    // Ray is parallel to the y-axis (if a is approximately zero)
    if (fabs(a) >= EPSILON)
    {
        float b = 2 * (transformed_ray.origin.x * transformed_ray.direction.x +
                       transformed_ray.origin.z * transformed_ray.direction.z);
        float c = transformed_ray.origin.x * transformed_ray.origin.x +
                  transformed_ray.origin.z * transformed_ray.origin.z - 1; // Assuming radius is 1
        // Calculate the discriminant
        float discriminant = b * b - 4 * a * c;
        // If the discriminant is negative, the ray does not intersect the cylinder
        if (discriminant >= 0)
        {
            // Calculate the two intersection points using the quadratic formula
            float sqrt_discriminant = sqrt(discriminant);
            float temp_t0 = (-b - sqrt_discriminant) / (2 * a);
            float temp_t1 = (-b + sqrt_discriminant) / (2 * a);
            // Ensure that temp_t0 is the smaller value and temp_t1 is the larger value
            if (temp_t0 > temp_t1)
            {
                float temp = temp_t0;
                temp_t0 = temp_t1;
                temp_t1 = temp;
            }
            // Check the first intersection point
            float y0 = transformed_ray.origin.y + temp_t0 * transformed_ray.direction.y;
            if (cyl.minimum < y0 && y0 < cyl.maximum)
            {
                *t0 = temp_t0;
                intersections++;
            }
            // Check the second intersection point
            float y1 = transformed_ray.origin.y + temp_t1 * transformed_ray.direction.y;
            if (cyl.minimum < y1 && y1 < cyl.maximum)
            {
                if (intersections == 0)
                {
                    *t0 = temp_t1;
                }
                else
                {
                    *t1 = temp_t1;
                }
                intersections++;
            }
        }
    }
    // ---- Intersect with the caps ----
    if (fabs(transformed_ray.direction.y) >= EPSILON)
    {
        // Check for intersection with lower end cap (y = cyl.minimum)
        float t_cap = (cyl.minimum - transformed_ray.origin.y) / transformed_ray.direction.y;
        if (t_cap >= 0 && check_cap(transformed_ray, t_cap))
        {
            if (intersections == 0)
            {
                *t0 = t_cap;
            }
            else if (intersections == 1)
            {
                *t1 = t_cap;
            }
            intersections++;
        }
        // Check for intersection with upper end cap (y = cyl.maximum)
        t_cap = (cyl.maximum - transformed_ray.origin.y) / transformed_ray.direction.y;
        if (t_cap >= 0 && check_cap(transformed_ray, t_cap))
        {
            if (intersections == 0)
            {
                *t0 = t_cap;
            }
            else if (intersections == 1)
            {
                *t1 = t_cap;
            }
            intersections++;
        }
    }
    return intersections;
}

void print_matrix(t_matrix *matrix)
{
    printf("Matrix (%d x %d):\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            printf("%f\t", matrix->data[i][j]); // Print each element
        }
        printf("\n"); // Newline after each row
    }
}*/

int plane_intersect(t_plane plane, t_ray r, float *t)
{
    float   plane_size;

    plane_size = 10;
    //print_matrix(plane.transform);
    // Transform ray into the plane's local space using the plane's inverse transform
    //t_tuple transformed_origin = apply_transformation(plane.inverse_transform, &r.origin);
    //t_tuple transformed_direction = apply_transformation(plane.inverse_transform, &r.direction);
    t_tuple transformed_origin = apply_transformation(plane.transform, &r.origin);
    t_tuple transformed_direction = apply_transformation(plane.transform, &r.direction);
    // Create a transformed ray from the transformed origin and direction
    t_ray transformed_ray = ray(transformed_origin, transformed_direction);
    // Avoid division by zero if the ray is parallel to the plane
    if (fabs(transformed_ray.direction.y) < EPSILON)
    {
        return 0;  // Ray is parallel to the plane, no intersection
    }
    // Calculate the intersection t value
    *t = -transformed_ray.origin.y / transformed_ray.direction.y;
    // Only accept intersections that occur "in front" of the ray origin
    if (*t >= 0)
    {
        // Check if the intersection point is within the bounds of the "finite" plane
        t_tuple intersection_point = position(transformed_ray, *t);

        // Limit the plane to a finite size (for testing purposes, e.g., a plane_size x plane_size square)
        if (fabs(intersection_point.x) <= plane_size / 2 && fabs(intersection_point.z) <= plane_size / 2)
        {
            return 1; // One intersection within the finite plane bounds
        }
    }
    return (0); // No intersection
}

int intersect(t_sphere sphere, t_ray ray, float *t0, float *t1) {
    t_tuple oc = tuple_subtract(ray.origin, sphere.center);
    float a = dot(ray.direction, ray.direction); // Should be 1 if normalized
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant >= 0)
    {
        *t0 = (-b - sqrt(discriminant)) / (2.0 * a);
        *t1 = (-b + sqrt(discriminant)) / (2.0 * a);

        // Check if the intersection points are valid in the ray's direction
        if (*t0 > 0 || *t1 > 0) {
            //printf("Intersection Points: t0 = %f, t1 = %f\n", *t0, *t1);
            return 1; // Intersection found
        }
    }
    // If you reach here, there are no valid intersections
    return 0; // No intersection
}

t_matrix* rotation_around_axis(t_tuple axis, float angle)
{
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float cos_theta = cos(angle);
    float sin_theta = sin(angle);
    float one_minus_cos = 1.0f - cos_theta;
    // Compute the components of the rotation matrix
    t_matrix* rotation = create_4x4_matrix(
        cos_theta + x * x * one_minus_cos,
        x * y * one_minus_cos - z * sin_theta,
        x * z * one_minus_cos + y * sin_theta,
        0,

        y * x * one_minus_cos + z * sin_theta,
        cos_theta + y * y * one_minus_cos,
        y * z * one_minus_cos - x * sin_theta,
        0,

        z * x * one_minus_cos - y * sin_theta,
        z * y * one_minus_cos + x * sin_theta,
        cos_theta + z * z * one_minus_cos,
        0,

        0, 0, 0, 1
    );
    return (rotation);
}

t_matrix* rotation_from_normal(t_tuple normal)
{
    t_tuple default_normal = vector(0, 1, 0);  // Default normal for the cylinder's y-axis
    t_tuple v1 = normalize(default_normal);
    t_tuple v2 = normalize(normal);

    // Check for axis-aligned cases directly
    if (fabs(v2.x - 1.0) < EPSILON && fabs(v2.y) < EPSILON && fabs(v2.z) < EPSILON)
    {
        // Align with the x-axis
        return rotation_around_axis(vector(0, 0, 1), -PI / 2);  // Rotate -90 degrees around the z-axis
    }
    else if (fabs(v2.z - 1.0) < EPSILON && fabs(v2.x) < EPSILON && fabs(v2.y) < EPSILON)
    {
        // Align with the z-axis (this seems to already be working correctly)
        return identity_matrix();  // No rotation needed, it's already aligned
    }

    // Compute the rotation axis using the cross product
    t_tuple axis = cross(v1, v2);
    float axis_length = magnitude(axis);

    // Handle the parallel/antiparallel case
    if (axis_length < EPSILON)
    {
        if (dot(v1, v2) > 0)
        {
            return identity_matrix();  // No rotation needed
        }
        else
        {
            // Rotate 180 degrees around any perpendicular axis
            t_tuple arbitrary_axis = vector(1, 0, 0);
            if (fabs(v1.x) > fabs(v1.y))
                arbitrary_axis = vector(0, 1, 0);
            axis = normalize(cross(v1, arbitrary_axis));
            return rotation_around_axis(axis, PI);  // Rotate 180 degrees
        }
    }

    axis = normalize(axis);
    float angle = acos(dot(v1, v2));
    return (rotation_around_axis(axis, angle));
}

/*
t_matrix* rotation_from_normal(t_tuple normal)
{
    t_tuple default_normal = vector(0, 1, 0);  // Default normal for the plane
    t_tuple v1 = normalize(default_normal);
    t_tuple v2 = normalize(normal);

    // Compute the rotation axis
    t_tuple axis = cross(v1, v2);
    float axis_length = magnitude(axis);

    // Check if the normals are parallel or antiparallel
    if (axis_length < EPSILON)
    {
        // Vectors are parallel or antiparallel
        if (dot(v1, v2) > 0)
        {
            // Vectors are parallel; no rotation needed
            return identity_matrix();
        }
        else
        {
            // Vectors are opposite; rotate 180 degrees around any perpendicular axis
            t_tuple arbitrary_axis = vector(1, 0, 0);
            if (fabs(v1.x) > fabs(v1.y))
                arbitrary_axis = vector(0, 1, 0);
            axis = normalize(cross(v1, arbitrary_axis));
            return rotation_around_axis(axis, PI); // Rotate 180 degrees
        }
    }
    axis = normalize(axis);
    float angle = acos(dot(v1, v2));
    // Create rotation matrix using Rodrigues' formula
    return (rotation_around_axis(axis, angle));
}*/

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
}

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

t_sphere sphere_create(t_tuple center, float radius, t_color col)
{
    printf("Creating sphere at (%f, %f, %f) with radius %f\n", center.x, center.y, center.z, radius);
    t_sphere sphere;

    //hittable_init(&sphere.base, sphere_hit);
    sphere.color = col;
    sphere.center = center;
    sphere.radius = fmax(0, radius);

     // Initialize transformation matrices
    sphere.translation_matrix = translation(center.x, center.y, center.z);
    sphere.rotation_matrix = identity_matrix(); // No rotation initially
    sphere.scaling_matrix = scaling(radius, radius, radius);
    printf("CHECK THIIIS!!!!\n");
    printf("Sphere Center Z: %f\n", center.z);
    printf("Translation Matrix Z-component: %f\n", sphere.translation_matrix->data[2][3]);

    // Combine transformations into one matrix
    sphere.transform = t_matrix_multiply(t_matrix_multiply(sphere.translation_matrix, sphere.rotation_matrix), sphere.scaling_matrix);

    // Calculate the inverse transform for ray-sphere intersection
    sphere.inverse_transform = inverse(sphere.transform);
    return (sphere);
}

t_light *point_light(t_tuple position, t_color intensity, float brightness)
{
    t_light *light = malloc(sizeof(t_light));
    if (!light) {
        // Handle allocation failure
        return NULL;
    }
    light->position = position;
    // Scale the intensity by the brightness ratio
    light->intensity = multiply_color_scalar(intensity, brightness);
    light->brightness = brightness;
    return (light);
}

t_light light_create(t_tuple position, t_color intensity)
{
    t_light light;
    light.position = position;
    light.intensity = intensity;
    return light;
}

// Function to multiply a tuple by a scalar
t_tuple tuple_multiply(t_tuple t, double scalar)
{
    return tuple(t.x * scalar, t.y * scalar, t.z * scalar, t.w);  // Preserves w
}

// Computes a point along the ray at parameter t
t_tuple position(t_ray r, double t)
{
    // p = origin + t * direction
    t_tuple scaled_direction = tuple_multiply(r.direction, t);
    return tuple_add(r.origin, scaled_direction);
}

void print_clock_face(t_var *var)
{
    float CLOCK_RADIUS = var->image_width * 1.0 / 8.0;
    float CENTER_X = var->image_width / (float)2.0;
    float CENTER_Y = var->image_height / (float)2.0;
    const int MARK_SIZE = 2; // Size of the mark, 10 pixels

    t_tuple twelve_oclock = point(0, 0, 1); // 12 o'clock at the top center
    int hours = 360;
    float angle_per_hour = (float)3.1415 / 180; // 2 * M_PI / 12

	printf("Center: (%f, %f)\n", CENTER_X, CENTER_Y);
	printf("Camera fov: %f\n", var->fov);

    // Draw each hour mark
    for (int hour = 0; hour < hours; hour++) {
        t_matrix *rotation = rotation_y(angle_per_hour * hour);
        t_tuple hour_pos = apply_transformation(rotation, &twelve_oclock);

    // Scale and translate position
    float px = CENTER_X + (float)(hour_pos.x * CLOCK_RADIUS);
    float py = CENTER_Y - (float)(hour_pos.z * CLOCK_RADIUS);  // Adjust y-axis if necessary

    printf("Hour %d: px = %f, py = %f\n", hour, px, py);

        // Draw a small square around the hour mark
        for (int dx = -MARK_SIZE / 2; dx <= MARK_SIZE / 2; dx++) {
            for (int dy = -MARK_SIZE / 2; dy <= MARK_SIZE / 2; dy++) {
                int final_px = px + dx;
                int final_py = py + dy;
                if (final_px >= 0 && final_px < var->image_width && final_py >= 0 && final_py < var->image_height) {
                    write_color(t_color_create(0, 0, 0), var, final_px, final_py);  // Draw black mark
                }
            }
        }
        free(rotation); // Free the matrix after using it
    }
}

void test_position_function()
{
    t_ray r;
    r.origin = point(2, 3, 4);
    r.direction = vector(1, 0, 0);
    // Test different values of t
    t_tuple expected;
    t_tuple result;
    // t = 0
    result = position(r, 0);
    expected = point(2, 3, 4);
    assert(tuple_equal(result, expected));
    // t = 1
    result = position(r, 1);
    expected = point(3, 3, 4);
    assert(tuple_equal(result, expected));
    // t = -1
    result = position(r, -1);
    expected = point(1, 3, 4);
    assert(tuple_equal(result, expected));
    // t = 2.5
    result = position(r, 2.5);
    expected = point(4.5, 3, 4);
    assert(tuple_equal(result, expected));

    printf("All position function tests passed.\n");
}

// Function to create a new ray given an origin and a direction
t_ray ray(t_tuple origin, t_tuple direction)
{
    t_ray new_ray;
    new_ray.origin = origin;
    new_ray.direction = direction;
    return new_ray;
}

// Multiplies a transformation matrix by a tuple and returns the transformed tuple
t_tuple apply_transformation(t_matrix *transformation, t_tuple *point)
{
    t_matrix *point_matrix = tuple_to_matrix(point);
    t_matrix *transformed_matrix = t_matrix_multiply(transformation, point_matrix);
    t_tuple transformed_point = matrix_to_tuple(transformed_matrix);
    free(point_matrix);
    free(transformed_matrix);
    return transformed_point;
}

// Function to convert the first column of a 4x4 matrix back to a t_tuple
t_tuple matrix_to_tuple(t_matrix *m)
{
    return tuple(m->data[0][0], m->data[1][0], m->data[2][0], m->data[3][0]);
}

// Function to convert a t_tuple to a 4x4 matrix for transformation
t_matrix *tuple_to_matrix(t_tuple *t)
{
    t_matrix *m = create_4x4_matrix(
        t->x, 0, 0, 0,
        t->y, 0, 0, 0,
        t->z, 0, 0, 0,
        t->w, 0, 0, 0
    );
    return m;
}

// Function to create a shearing matrix
t_matrix* shearing(float xy, float xz, float yx, float yz, float zx, float zy)
{
    t_matrix* shear = identity_matrix(); // Initialize as identity matrix

    // Set the shearing components
    shear->data[0][1] = xy;
    shear->data[0][2] = xz;
    shear->data[1][0] = yx;
    shear->data[1][2] = yz;
    shear->data[2][0] = zx;
    shear->data[2][1] = zy;

    return shear;
}

// Function to create a rotation matrix around the Z-axis
t_matrix *rotation_z(float radians)
{
    t_matrix* rotation = identity_matrix(); // Initialize as identity matrix

    // Set the rotation components
    rotation->data[0][0] = cos(radians);
    rotation->data[0][1] = -sin(radians);
    rotation->data[1][0] = sin(radians);
    rotation->data[1][1] = cos(radians);

    return rotation;
}

// Function to create a rotation matrix around the Y-axis
t_matrix* rotation_y(float radians)
{
    t_matrix *transform = identity_matrix(); // Initialize as identity matrix

    // Set the rotation components
    transform->data[0][0] = cos(radians);
    transform->data[0][2] = sin(radians);
    transform->data[2][0] = -sin(radians);
    transform->data[2][2] = cos(radians);

    return transform;
}

// Function to create a rotation matrix for rotating around the X axis
t_matrix *rotation_x(float radians)
{
    t_matrix *transform = identity_matrix();

    // Set the rotation components for the X axis
    transform->data[1][1] = cos(radians);
    transform->data[1][2] = -sin(radians);
    transform->data[2][1] = sin(radians);
    transform->data[2][2] = cos(radians);

    return transform;
}

// Function to create a reflection matrix (scaling by a negative value)
t_matrix *reflective_scaling(float x, float y, float z)
{
    // Use the scaling function to scale by the negative values for reflection
    return scaling(x, y, z);
}

// Function to compute the inverse of a scaling matrix
t_matrix *inverse_scaling(float x, float y, float z)
{
    // Inverse scaling factors are the reciprocals of the original scaling factors
    return scaling(1.0f / x, 1.0f / y, 1.0f / z);
}

// Function to create a 4x4 scaling matrix using while loops
t_matrix *scaling(float x, float y, float z)
{
    t_matrix *transform = (t_matrix *)malloc(sizeof(t_matrix));
    if (transform == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    transform->rows = 4;
    transform->cols = 4;
    int i = 0, j;
    while (i < 4) {
        j = 0;
        while (j < 4) {
            transform->data[i][j] = (i == j) ? 1.0f : 0.0f;
            j++;
        }
        i++;
    }
    // Set scaling components
    transform->data[0][0] = x;
    transform->data[1][0] = 0;
    transform->data[2][0] = 0;

    transform->data[0][1] = 0;
    transform->data[1][1] = y;
    transform->data[2][1] = 0;

    transform->data[0][2] = 0;
    transform->data[1][2] = 0;
    transform->data[2][2] = z;

    return (transform);
}

// Function to compare two tuples (for test validation)
bool tuple_equal(t_tuple t1, t_tuple t2)
{
    return equal(t1.x, t2.x) && equal(t1.y, t2.y) && equal(t1.z, t2.z) && equal(t1.w, t2.w);
}

// Function to create the inverse of a translation matrix using while loops
t_matrix *inverse_translation(t_matrix *transform)
{
    return translation(-transform->data[0][3], -transform->data[1][3], -transform->data[2][3]);
}

// Function to multiply a 4x4 matrix by a point (assumed to be a 4x1 vector)
t_tuple multiply_matrix_tuple(t_matrix *m, t_tuple *p) {
    t_tuple result;

    result.x = m->data[0][0] * p->x + m->data[0][1] * p->y + m->data[0][2] * p->z + m->data[0][3] * p->w;
    result.y = m->data[1][0] * p->x + m->data[1][1] * p->y + m->data[1][2] * p->z + m->data[1][3] * p->w;
    result.z = m->data[2][0] * p->x + m->data[2][1] * p->y + m->data[2][2] * p->z + m->data[2][3] * p->w;
    result.w = m->data[3][0] * p->x + m->data[3][1] * p->y + m->data[3][2] * p->z + m->data[3][3] * p->w;

    return (result);
}

// Function to create a 4x4 translation matrix
t_matrix *translation(float x, float y, float z)
{
    // Initialize the identity matrix
    t_matrix *transform = identity_matrix();

    // Set the translation components
    transform->data[0][3] = x;
    transform->data[1][3] = y;
    transform->data[2][3] = z;

    return (transform);
}

t_matrix *inverse(t_matrix *m)
{
    // Calculate the determinant first
    float det = determinant(m);
    if (det == 0.0f)
    {
        printf("Matrix is not invertible.\n");
        exit(1);  // You can handle it better based on your needs.
    }
    // Get the cofactor matrix
    t_matrix *cofactor_m = cofactor_matrix(m);
    // Get the adjugate matrix (transpose of the cofactor matrix)
    t_matrix *adjugate_m = transpose(cofactor_m);
    // Create the inverse matrix by dividing the adjugate matrix by the determinant
    t_matrix *inverse_m = (t_matrix *)malloc(sizeof(t_matrix));
    inverse_m->rows = m->rows;
    inverse_m->cols = m->cols;
    for (int i = 0; i < inverse_m->rows; i++)
    {
        for (int j = 0; j < inverse_m->cols; j++)
        {
            inverse_m->data[i][j] = adjugate_m->data[i][j] / det;
        }
    }
    // Free allocated memory for cofactor and adjugate matrices
    free(cofactor_m);
    free(adjugate_m);
    return (inverse_m);
}

t_matrix* cofactor_matrix(const t_matrix *m)
{
    // Allocate memory for the cofactor matrix
    t_matrix *cofactor_m = (t_matrix *)malloc(sizeof(t_matrix));
    if (cofactor_m == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    // Initialize the cofactor matrix with the same dimensions as the original matrix
    cofactor_m->rows = m->rows;
    cofactor_m->cols = m->cols;
    // Iterate over each element using while loops
    int i = 0;
    while (i < m->rows) {
        int j = 0;
        while (j < m->cols) {
            // Calculate the cofactor for element (i, j)
            cofactor_m->data[i][j] = cofactor(m, i, j);
            j++;
        }
        i++;
    }
    return (cofactor_m);
}

// Function to check if matrix is invertible
bool is_invertible(t_matrix *m)
{
    float det = determinant(m);
    return det != 0;
}

// Function to compute the cofactor of an matrix element
float cofactor(const t_matrix *m, int row, int col)
{
    // Calculate the minor at the given row and column
    float minor_value = minor(m, row, col);

    // Determine if the cofactor needs to be negated
    if ((row + col) % 2 != 0)
    {
        return -minor_value;
    }
    return minor_value;
}

// Function to return submatrix of a given matrix
t_matrix* submatrix(const t_matrix *m, int remove_row, int remove_col)
{
    int new_rows = m->rows - 1;
    int new_cols = m->cols - 1;

    // Create a new matrix with reduced size
    t_matrix *sub_m = (t_matrix *)malloc(sizeof(t_matrix));
    if (sub_m == NULL)
    {
        // Handle memory allocation failure
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    sub_m->rows = new_rows;
    sub_m->cols = new_cols;

    int i = 0, j = 0, row_offset = 0, col_offset = 0;

    while (i < new_rows)
    {
        if (i >= remove_row)
        {
            row_offset = 1;
        }
        j = 0;
        col_offset = 0;

        while (j < new_cols)
        {
            if (j >= remove_col)
            {
                col_offset = 1;
            }
            sub_m->data[i][j] = m->data[i + row_offset][j + col_offset];
            j++;
        }
        i++;
    }
    return sub_m;
}

// Function to return the dminor/determinant of a given matrix
float minor(const t_matrix *m, int row, int col)
{
    // Get the submatrix by removing the specified row and column
    t_matrix *sub_m = submatrix(m, row, col);

    // Calculate the determinant of the submatrix
    float det = determinant(sub_m);

    // Free the submatrix memory
    free(sub_m);

    // Return the determinant, which is the minor
    return det;
}

// Function to return the determinant of a 2x2 matrix
float determinant_2x2(const t_matrix *m)
{
    return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
}

// Function to return the determinant of a 3x3 matrix
float determinant_3x3(const t_matrix *m)
{
    return m->data[0][0] * (m->data[1][1] * m->data[2][2] - m->data[1][2] * m->data[2][1]) -
           m->data[0][1] * (m->data[1][0] * m->data[2][2] - m->data[1][2] * m->data[2][0]) +
           m->data[0][2] * (m->data[1][0] * m->data[2][1] - m->data[1][1] * m->data[2][0]);
}

// Function to return the determinant of a matrix
float determinant(const t_matrix *m)
{
    if (m->rows == 2 && m->cols == 2)
    {
        return determinant_2x2(m);
    }
    else if (m->rows == 3 && m->cols == 3)
    {
        return determinant_3x3(m);
    }
    else if (m->rows == 4 && m->cols == 4)
    {
        float det = 0.0;
        for (int col = 0; col < m->cols; col++)
        {
            det += m->data[0][col] * cofactor(m, 0, col);
        }
        return det;
    }
    else
    {
        printf("Determinant calculation not implemented for matrices larger than 4x4.\n");
        return 0.0f;
    }
}

// Function to create an identity matrix
t_matrix* identity_matrix()
{
    t_matrix *identity = (t_matrix *)malloc(sizeof(t_matrix));
    if (identity == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    identity->rows = 4;
    identity->cols = 4;
    int i = 0;
    while (i < 4)
    {
        int j = 0;
        while (j < 4)
        {
            if (i == j)
            {
                identity->data[i][j] = 1.0;
            }
            else
            {
                identity->data[i][j] = 0.0;
            }
            j++;
        }
        i++;
    }
    return (identity);
}

// Function to transpose a matrix
t_matrix* transpose(t_matrix *m)
{
    // Allocate memory for the transposed matrix
    t_matrix *transposed = (t_matrix *)malloc(sizeof(t_matrix));
    if (transposed == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    transposed->rows = m->cols;
    transposed->cols = m->rows;
    int i = 0;
    while (i < m->rows)
    {
        int j = 0;
        while (j < m->cols)
        {
            // Swap rows and columns
            transposed->data[j][i] = m->data[i][j];
            j++;
        }
        i++;
    }
    return (transposed);
}

// Function to multiply two 4x4 matrices
t_matrix* t_matrix_multiply(t_matrix *a, t_matrix *b)
{
    // Ensure both matrices are 4x4
    if (a->rows != 4 || a->cols != 4 || b->rows != 4 || b->cols != 4)
    {
        printf("Error: Both matrices must be 4x4.\n");
        return NULL;
    }
    // Allocate memory for the result matrix
    t_matrix *result = (t_matrix *)malloc(sizeof(t_matrix));
    if (result == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    result->rows = 4;
    result->cols = 4;
    // Perform matrix multiplication
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->data[i][j] = 0; // Initialize the element
            for (int k = 0; k < 4; k++)
            {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return (result);
}

t_matrix *create_2x2_matrix(float a, float b, float c, float d)
{
    t_matrix *m = (t_matrix *)malloc(sizeof(t_matrix));
    if (m == NULL)
    {
        // Handle memory allocation failure
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    m->rows = 2;
    m->cols = 2;
    m->data[0][0] = a;
    m->data[0][1] = b;
    m->data[1][0] = c;
    m->data[1][1] = d;
    return (m);
}

t_matrix *create_3x3_matrix(float a, float b, float c,
                           float d, float e, float f,
                           float g, float h, float i)
{
    t_matrix *m = (t_matrix *)malloc(sizeof(t_matrix));
    if (m == NULL)
    {
        // Handle memory allocation failure
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    m->rows = 3;
    m->cols = 3;
    m->data[0][0] = a;
    m->data[0][1] = b;
    m->data[0][2] = c;
    m->data[1][0] = d;
    m->data[1][1] = e;
    m->data[1][2] = f;
    m->data[2][0] = g;
    m->data[2][1] = h;
    m->data[2][2] = i;
    return (m);
}

t_matrix *create_4x4_matrix(float a, float b, float c, float d,
                           float e, float f, float g, float h,
                           float i, float j, float k, float l,
                           float m, float n, float o, float p)
{
    t_matrix *matrix = (t_matrix *)malloc(sizeof(t_matrix));
    if (matrix == NULL)
    {
        // Handle memory allocation failure
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    matrix->rows = 4;
    matrix->cols = 4;
    matrix->data[0][0] = a;
    matrix->data[0][1] = b;
    matrix->data[0][2] = c;
    matrix->data[0][3] = d;
    matrix->data[1][0] = e;
    matrix->data[1][1] = f;
    matrix->data[1][2] = g;
    matrix->data[1][3] = h;
    matrix->data[2][0] = i;
    matrix->data[2][1] = j;
    matrix->data[2][2] = k;
    matrix->data[2][3] = l;
    matrix->data[3][0] = m;
    matrix->data[3][1] = n;
    matrix->data[3][2] = o;
    matrix->data[3][3] = p;
    return matrix;
}

float t_matrix_get(t_matrix *m, int row, int col)
{
    // Ensure the row and column indices are within the valid range
    if (row >= 0 && row < m->rows && col >= 0 && col < m->cols)
    {
        printf("%f\n", m->data[row][col]);
        return m->data[row][col];
    }
    else
    {
        // Handle invalid indices, e.g., returning 0 or some error value
        // You could also print an error message or return a special value like NAN
        printf("Error: Index out of bounds.\n");
        return 0; // or NAN, depending on your needs
    }
}

// Function to compare two floating-point numbers for equality
bool equal(double a, double b)
{
    return fabs(a - b) < EPSILON;
}

// Function to compare two matrices for equality
int t_matrix_equal(t_matrix *a, t_matrix *b)
{
    // First, check if the dimensions of the matrices are the same
    if (a->rows != b->rows || a->cols != b->cols)
    {
        return 0; // Matrices are not equal
    }

    // Compare each element in the matrices
    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < a->cols; j++)
        {
            if (!equal(a->data[i][j], b->data[i][j]))
            {
                return 0; // Matrices are not equal
            }
        }
    }
    return 1; // Matrices are equal
}

// Function to create a tuple
t_tuple tuple(double x, double y, double z, double w)
{
    t_tuple t;
    t.x = x;
    t.y = y;
    t.z = z;
    t.w = w;
    return (t);
}
/*
t_color hadamard_product(t_color c1, t_color c2)
{
    t_color result;
    result.red = c1.red * c2.red;
    result.green = c1.green * c2.green;
    result.blue = c1.blue * c2.blue;
    return result;
}*/

// Function to compare two colors
bool compare_colors(t_color c1, t_color c2)
{
    return equal(c1.r, c2.r) && equal(c1.g, c2.g) && equal(c1.b, c2.b);
}

// Function to add two colors
t_color add_colors(t_color c1, t_color c2)
{
    return (t_color){c1.r + c2.r, c1.g + c2.g, c1.b + c2.b};
}

// Function to subtract two colors
t_color subtract_colors(t_color c1, t_color c2)
{
    return (t_color){c1.r - c2.r, c1.g - c2.g, c1.b - c2.b};
}

// Function to multiply a color by a scalar
t_color multiply_color_scalar(t_color c, double scalar)
{
    return (t_color){c.r * scalar, c.g * scalar, c.b * scalar};
}

// Function to multiply two colors (Hadamard product)
t_color multiply_colors(t_color c1, t_color c2)
{
    return (t_color){c1.r * c2.r, c1.g * c2.g, c1.b * c2.b};
}

// Function to print a color (for debugging)
void print_color(t_color c)
{
    printf("(%.2f, %.2f, %.2f)\n", c.r, c.g, c.b);
}

// Function to create a point (w = 1.0)
t_tuple point(double x, double y, double z)
{
    return tuple(x, y, z, 1.0);
}

// Function to create a vector (w = 0.0)
t_tuple vector(double x, double y, double z)
{
    return tuple(x, y, z, 0.0);
}

// Function to check if a tuple is a point (w = 1.0)
bool is_point(t_tuple t)
{
    return t.w == 1.0;
}

// Function to check if a tuple is a vector (w = 0.0)
bool is_vector(t_tuple t)
{
    return t.w == 0.0;
}

// Function to add two tuples
t_tuple tuple_add(t_tuple t1, t_tuple t2)
{
    return tuple(t1.x + t2.x, t1.y + t2.y, t1.z + t2.z, t1.w + t2.w);
}

// Function to subtract two tuples
t_tuple tuple_subtract(t_tuple t1, t_tuple t2)
{
    return tuple(t1.x - t2.x, t1.y - t2.y, t1.z - t2.z, t1.w - t2.w);
}

// Function to subtract a vector from a point
t_tuple subtract_vector_from_point(t_tuple point, t_tuple vector)
{
    // Ensure the w component remains 1.0 for a point
    return tuple(point.x - vector.x, point.y - vector.y, point.z - vector.z, 1.0);
}

// Function to negate a vector (subtract it from the zero vector)
t_tuple negate_vector(t_tuple v)
{
    t_tuple zero = vector(0, 0, 0);
    return tuple_subtract(zero, v);
}

// Function to negate a tuple
t_tuple negate_tuple(t_tuple t)
{
    return tuple(-t.x, -t.y, -t.z, -t.w);
}

// Function to divide a tuple by a scalar, same as multiplying with a fraction lets say 0.5
t_tuple tuple_divide(t_tuple t, double scalar)
{
    if (scalar == 0)
    {
        // Handle division by zero if necessary
        // Here we'll just return a tuple with NaN components as a placeholder
        return tuple(NAN, NAN, NAN, NAN);
    }
    return tuple(t.x / scalar, t.y / scalar, t.z / scalar, t.w / scalar);
}

// Function to calculate the magnitude of a vector
double magnitude(t_tuple v)
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

// Function to compare magnitude results
bool magnitude_equal(t_tuple v, double expected_magnitude)
{
    return equal(magnitude(v), expected_magnitude);
}

// Function to normalize a vector
t_tuple normalize(t_tuple v)
{
    double mag = magnitude(v);
    if (mag == 0) {
        // Handling the case where magnitude is zero (though it's not expected for valid vectors)
        return vector(0, 0, 0);
    }
    return vector(v.x / mag, v.y / mag, v.z / mag);
}

// Function to compute the dot product of two vectors
double dot(t_tuple a, t_tuple b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Function to compute the cross product of two vectors
t_tuple cross(t_tuple a, t_tuple b)
{
    return vector(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Helper function to compare two matrices
int matrices_are_equal(t_matrix *m1, t_matrix *m2) {
    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        return 0;
    }

    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m1->cols; j++) {
            if (!equal(m1->data[i][j], m2->data[i][j])) {
                return 0;
            }
        }
    }
    return 1;
}
void init_ambient_color(t_var *var, t_map *map)
{
    t_color ambient = t_color_create(map->ambient->r,map->ambient->b,map->ambient->g);
    var->ambientl = multiply_color_scalar(ambient,map->ambient->ratio);
}

void init_test_lights(t_var *var, t_map *map)
{
    int i = 0;
    t_lights *current_light = map->lights;
    var->num_lights = map->element_count->light;
    var->test_light = malloc(var->num_lights * sizeof(t_light));
    if (!var->test_light)
    {
        // Handle malloc failure (optional)
        return;
    }
    while (current_light != NULL)
    {
        t_tuple position = point(current_light->x, current_light->y, current_light->z);
        t_color intensity = t_color_create(current_light->r, current_light->b, current_light->g);
        var->test_light[i] = light_create(position, intensity);
        //var->test_light[i].direction = normalize((t_tuple){0.0f, -1.0f, 0.0f}); // Example direction pointing downwards
        //var->test_light[i].cutoff_angle = 30.0f; // Example cutoff angle in degrees
        i++;
        current_light = current_light->next;
    }
}

void init_test_cylinders(t_var *var, t_map *map)
{
    int i = 0;
    t_cylinders *current_cylinder = map->cylinders;
    var->num_cylinders = map->element_count->cylinder;
    var->test_cylinder = malloc(var->num_cylinders * sizeof(t_cylinder));
    if (!var->test_cylinder)
    {
        // Handle malloc failure (optional)
        return;
    }
    while (current_cylinder != NULL)
    {
        t_tuple center = point(current_cylinder->x, current_cylinder->y, current_cylinder->z);
        t_color color = t_color_create(current_cylinder->r, current_cylinder->b, current_cylinder->g);
        t_tuple orientation = vector(current_cylinder->nx, current_cylinder->ny, current_cylinder->nz);
        var->test_cylinder[i] = cylinder_create(center, current_cylinder->diameter, current_cylinder->height, color, orientation);
        i++;
        current_cylinder = current_cylinder->next;
    }
}

void init_test_planes(t_var *var, t_map *map)
{
    int i = 0;
    t_planes *current_plane = map->planes;  // Assuming map has a 'planes' linked list
    var->num_planes = map->element_count->plane;  // Assuming map has an element count for planes
    var->test_plane = malloc(var->num_planes * sizeof(t_plane));
    if (!var->test_plane)
    {
        // Handle malloc failure (optional)
        return;
    }

    while (current_plane != NULL)
    {
        t_tuple center = point(current_plane->x, current_plane->y, current_plane->z);
        t_color color = t_color_create(current_plane->r, current_plane->b, current_plane->g);
        t_tuple orientation = vector(current_plane->nx, current_plane->ny, current_plane->nz);
        //var->test_plane[i] = plane_create(center, normal, color);
        var->test_plane[i] = plane_create(center, color, orientation);
        i++;
        current_plane = current_plane->next;  // Move to the next plane in the list
    }
}

void init_test_sphere(t_var *var, t_map *map)
{
    int i = 0;
    t_spheres *current_sphere = map->spheres;  // Create a temporary pointer to traverse the list
    var->num_spheres = map->element_count->sphere;
    var->test_sphere = malloc(var->num_spheres * sizeof(t_sphere));
    if (!var->test_sphere)
    {
        // Handle malloc failure (optional)
        return;
    }

    while (current_sphere != NULL)
    {
        t_tuple center = point(current_sphere->x, current_sphere->y, current_sphere->z);
        float size = current_sphere->diameter;
        t_color color = t_color_create(current_sphere->r, current_sphere->b, current_sphere->g);
        var->test_sphere[i] = sphere_create(center, size, color);
        i++;
        current_sphere = current_sphere->next;  // Move to the next sphere in the list
    }
}

t_tuple calculate_sphere_normal(const t_sphere *sphere, const t_tuple *point) {
    return normalize(tuple_subtract(*point, sphere->center));
}

t_tuple ray_at(t_ray ray, float t)
{
    return tuple_add(ray.origin, tuple_multiply(ray.direction, t));
}

// Function to convert degrees to radians
float degrees_to_radians(float degrees) {
    return degrees * (PI / 180.0f);
}

t_color calculate_lighting(const t_sphere *sphere, const t_light *light, const t_tuple *point, const t_tuple *normal)
{
    // Calculate the direction from the light to the point
    t_tuple light_dir = normalize(tuple_subtract(light->position, *point));

    // Calculate the lighting based on the light's intensity and the object's color
    float intensity_factor = fmax(dot(*normal, light_dir), 0.0f);
    t_color light_effect = multiply_color_scalar(light->intensity, intensity_factor);
    t_color final_color = multiply_colors(sphere->color, light_effect);

    return final_color;
}

// Function to calculate the normal vector at a point on the cylinder's surface
t_tuple calculate_cylinder_normal(const t_cylinder *cylinder, const t_tuple *point)
{
    t_tuple center_to_point = tuple_subtract(*point, cylinder->center);
    t_tuple normal = tuple(center_to_point.x, 0.0f, center_to_point.z,0); // Project onto xz-plane
    return normalize(normal);
}

// Function to calculate lighting for a cylinder
t_color calculate_lighting_cy(const t_cylinder *cylinder, const t_light *light, const t_tuple *point) {
    // Calculate the normal vector at the point on the cylinder's surface
    t_tuple normal = calculate_cylinder_normal(cylinder, point);

    // Calculate the direction from the light to the point
    t_tuple light_dir = normalize(tuple_subtract(light->position, *point));

    // Calculate the lighting based on the light's intensity and the object's color
    float intensity_factor = fmax(dot(normal, light_dir), 0.0f);
    t_color light_effect = multiply_color_scalar(light->intensity, intensity_factor);
    t_color final_color = multiply_colors(cylinder->color, light_effect);

    return final_color;
}

bool intersect_sphere(const t_ray *ray, const t_sphere *sphere, float *t) {
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

bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    // Cylinder's axis is assumed to be aligned with the y-axis
    t_tuple oc = tuple_subtract(ray->origin, cylinder->center);
    t_tuple direction = ray->direction;
    t_tuple axis = {0, 1, 0, 1}; // y-axis

    float a = dot(direction, direction) - pow(dot(direction, axis), 2);
    float b = 2.0 * (dot(direction, oc) - dot(direction, axis) * dot(oc, axis));
    float c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    } else {
        *t = (-b - sqrt(discriminant)) / (2.0 * a);
        return (*t >= 0);
    }
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

bool intersect_object(const t_ray *ray, const t_object *object, float *t)
{
    if (object->type == SPHERE)
    {
        return intersect_sphere(ray, &object->data.sphere, t);
    }
    else if (object->type == CYLINDER)
    {
        return intersect_cylinder(ray, &object->data.cylinder, t);
    }
    else if (object->type == PLANE)
    {
        return intersect_plane(ray, &object->data.plane, t);
    }
    else
    {
        return false;
    }
}

bool find_closest_intersection(const t_ray *ray, t_object *objects, int num_objects, t_object **closest_object, float *closest_t)
{
    *closest_t = FLT_MAX;
    bool hit = false;

    for (int i = 0; i < num_objects; i++)
    {
        float t;
        if (intersect_object(ray, &objects[i], &t) && t < *closest_t) {
            *closest_t = t;
            *closest_object = &objects[i];
            hit = true;
        }
    }

    return hit;
}

bool is_in_shadow(const t_tuple *point, const t_light *light, const t_object *objects, int num_objects) {
    t_tuple light_dir = normalize(tuple_subtract(light->position, *point));
    t_tuple shadow_origin = tuple_add(*point, tuple_multiply(light_dir, 0.001f)); // Add small bias to avoid self-intersection
    t_ray shadow_ray = {shadow_origin, light_dir};
    float t;

    for (int i = 0; i < num_objects; i++)
    {
        if (intersect_object(&shadow_ray, &objects[i], &t) && t > 0.001f)
        {
            return true;
        }
    }

    return false;
}

t_color calculate_phong_lighting(const t_tuple *point, const t_tuple *normal, const t_light *light, const t_color *object_color, const t_tuple *view_dir, const t_object *objects, int num_objects)
{

    t_color ambient = multiply_color_scalar(light->intensity, 0.1f); // Ambient component

    t_tuple light_dir = normalize(tuple_subtract(light->position, *point));
    float diff = fmax(dot(*normal, light_dir), 0.0f);
    t_color diffuse = multiply_color_scalar(light->intensity, diff); // Diffuse component

    t_tuple reflect_dir = tuple_subtract(tuple_multiply(*normal, 2 * dot(*normal, light_dir)), light_dir);
    float spec = pow(fmax(dot(*view_dir, reflect_dir), 0.0f), 32);
    t_color specular = multiply_color_scalar(light->intensity, spec); // Specular component

    t_color final_color = multiply_colors(*object_color, ambient);
     if (!is_in_shadow(point, light, objects, num_objects)) {
        final_color = color_add(final_color, multiply_colors(*object_color, diffuse));
        final_color = color_add(final_color, specular);
    }

    return final_color;
}

t_tuple calculate_normal(const t_object *object, const t_tuple *point)
{
    if (object->type == SPHERE) {
        return calculate_sphere_normal(&object->data.sphere, point);
    } else if (object->type == CYLINDER) {
        return calculate_cylinder_normal(&object->data.cylinder, point);
    } else if (object->type == PLANE)
    {
        return object->data.plane.normal;
    } else {
        return (t_tuple){0, 0, 0,0};
    }
}

void printimage(void *param) {
    t_var *var = param;

    for (int y = 0; y < (int)var->image_height; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            float u = (float)x / (float)(SCREEN_WIDTH - 1);
            float v = (float)y / (float)(var->image_height - 1);

            // Calculate ray direction for each pixel
            t_tuple ray_direction = normalize(tuple_subtract(
                tuple_add(var->cam.lower_left_corner,
                          tuple_add(tuple_multiply(var->cam.horizontal, u),
                                    tuple_multiply(var->cam.vertical, v))),
                var->cam.position));

            t_ray r = ray(var->cam.position, ray_direction);

            // Initialize color to background color
            t_color pixel_color = var->ambientl;

            // Check for intersections with objects
            //float t;
            t_tuple intersection_point;
            t_tuple normal;
            t_color object_color;
            bool hit = false;
             // Find the closest intersection
            t_object *closest_object = NULL;
            float closest_t;

            for (int i = 0; i < var->num_objects; i++)
            {
                if (find_closest_intersection(&r, var->objects, var->num_objects, &closest_object, &closest_t))
                {
                    intersection_point = tuple_add(r.origin, tuple_multiply(r.direction, closest_t));
                    normal = calculate_normal(&var->objects[i], &intersection_point);
                    if(closest_object->type ==  SPHERE)
                        object_color = closest_object->data.sphere.color; // Adjust based on object type
                    if(closest_object->type ==  CYLINDER)
                        object_color = closest_object->data.cylinder.color;
                    if(closest_object->type ==  PLANE)
                        object_color = closest_object->data.plane.color;
                    hit = true;
                    break;
                }
            }

            // Calculate lighting if an intersection was found
            if (hit)
            {
                t_tuple view_dir = normalize(tuple_subtract(var->cam.position, intersection_point));
                //print_color_values(object_color);

                pixel_color = calculate_phong_lighting(&intersection_point, &normal, &var->test_light[0], &object_color, &view_dir, var->objects, var->num_objects);
            }

            // Write the pixel color to the image
            write_color(pixel_color, var, x, y);
        }
    }
}

void test_point_light_attributes()
{
    t_color intensity = t_color_create(1, 1, 1);
    t_tuple position = point(0, 0, 0);
    float brightness = 1.0f; // Full brightness

    t_light *light = point_light(position, intensity, brightness);

    if (tuple_equal(light->position, position) &&
        compare_colors(light->intensity, intensity)) {
        printf("Test Passed: Point light has correct position and intensity.\n");
    } else {
        printf("Test Failed: Point light attributes are incorrect.\n");
    }

    // Clean up
    free(light);
}

void initialize_scene(t_var *var, t_map *map)
{
    int obj_index = 0;

    var->num_objects = map->element_count->sphere + map->element_count->cylinder + map->element_count->plane;
    var->objects = malloc(var->num_objects * sizeof(t_object));
    if (var->objects == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Initialize spheres
    t_spheres *current_sphere = map->spheres;
    while (current_sphere != NULL)
    {
        t_tuple center = point(current_sphere->x, current_sphere->y, current_sphere->z);
        float radius = current_sphere->diameter / 2.0f; // Assuming diameter is provided
        t_color color = t_color_create(current_sphere->r, current_sphere->g, current_sphere->b);

        var->objects[obj_index].type = SPHERE;
        var->objects[obj_index].data.sphere.center = center;
        var->objects[obj_index].data.sphere.radius = radius;
        var->objects[obj_index].data.sphere.color = color;

        current_sphere = current_sphere->next;
        obj_index++;
    }

    // Initialize cylinders
    t_cylinders *current_cylinder = map->cylinders;
    while (current_cylinder != NULL)
    {
        t_tuple center = point(current_cylinder->x, current_cylinder->y, current_cylinder->z);
        float radius = current_cylinder->diameter / 2.0f; // Assuming diameter is provided
        float height = current_cylinder->height;
        t_color color = t_color_create(current_cylinder->r, current_cylinder->g, current_cylinder->b);

        var->objects[obj_index].type = CYLINDER;
        var->objects[obj_index].data.cylinder.center = center;
        var->objects[obj_index].data.cylinder.radius = radius;
        var->objects[obj_index].data.cylinder.height = height;
        var->objects[obj_index].data.cylinder.color = color;

        current_cylinder = current_cylinder->next;
        obj_index++;
    }

    // Initialize planes
    t_planes *current_plane = map->planes;
    while (current_plane != NULL)
    {
        t_tuple point = tuple(current_plane->x, current_plane->y, current_plane->z, 1.0f);
        t_tuple normal = vector(current_plane->nx, current_plane->ny, current_plane->nz);
        t_color color = t_color_create(current_plane->r, current_plane->g, current_plane->b);

        var->objects[obj_index].type = PLANE;
        var->objects[obj_index].data.plane.point = point;
        var->objects[obj_index].data.plane.normal = normal;
        var->objects[obj_index].data.plane.color = color;

        current_plane = current_plane->next;
        obj_index++;
    }
}

void free_scene(t_var *var)
{
    printf("Freeing scene...\n");
    if (var->objects != NULL)
    {
        free(var->objects);
        var->objects = NULL;
    }
}

int main(int argc, char **argv)
{
	t_var var;
	t_element_count element_count;
	t_map *map;

	if (argc != 2)
	{
		ft_printf("Error in arguments, just give a map filename\n");
		return (0);
	}
	ft_memset(&element_count, 0, sizeof(t_element_count));
	map = malloc(sizeof(t_map));
	setup_data(&element_count, map);
	if (!map)
		return (0);
	if (read_to_parse(&element_count, map, argv) == 0)
		return (0);
	print_data(map);
	printf("image width: %f\n", var.image_width);
	printf("image height: %f\n", var.image_height);
	if (mlxinit(&var, map) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    init_ambient_color(&var, map);
    initialize_camera(&var, &var.cam, map);
    initialize_scene(&var, map);
    init_test_lights(&var, map); // TESTI LIGHT!!!!
	printimage(&var);
	hooks(&var);
	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	terminate_data(map, &var, "program ended successfully\n");
    free_scene(&var);
	return (EXIT_SUCCESS);
}
