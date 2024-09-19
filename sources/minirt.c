/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/09/10 18:52:32 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"
//#include "../includes/test_functions.h"

/*
int intersect(t_sphere sphere, t_ray ray, float *t0, float *t1) {
    // Calculate vector from sphere center to ray origin
    t_tuple oc = tuple_subtract(ray.origin, sphere.center);

    // Calculate coefficients for the quadratic formula
    float a = dot(ray.direction, ray.direction); // Assuming you have a `dot` function for t_tuple
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;

    // Calculate the discriminant
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return 0; // No intersection
    } else {
        float sqrt_discriminant = sqrt(discriminant);
        *t0 = (-b - sqrt_discriminant) / (2 * a);
        *t1 = (-b + sqrt_discriminant) / (2 * a);
        return discriminant == 0 ? 1 : 2; // 1 if tangent, 2 if intersects at two points
    }
}*/

int intersect(t_sphere s, t_ray r, float *t0, float *t1) {
    // Calculate vector from ray origin to sphere center
    t_tuple sphere_to_ray = tuple_subtract(r.origin, s.center);

    float a = dot(r.direction, r.direction);  // Always 1 since the direction is normalized
    float b = 2 * dot(sphere_to_ray, r.direction);
    float c = dot(sphere_to_ray, sphere_to_ray) - (s.radius * s.radius);
    float discriminant = (b * b) - (4 * a * c);

    if (discriminant < 0) {
        return 0;  // No intersection
    } else {
        *t0 = (-b - sqrt(discriminant)) / (2 * a);
        *t1 = (-b + sqrt(discriminant)) / (2 * a);
        return 1;  // Intersection found
    }
}

// int intersect(t_sphere s, t_ray r, float *t0, float *t1) {
//     // Sphere center at the origin for simplicity
//     t_tuple sphere_to_ray = tuple_subtract(r.origin, point(0, 0, -10));

//     float a = dot(r.direction, r.direction);
//     float b = 2 * dot(sphere_to_ray, r.direction);
//     float c = dot(sphere_to_ray, sphere_to_ray) - s.radius * s.radius;
//     float discriminant = b * b - 4 * a * c;

//     if (discriminant < 0) {
//         return 0;  // No intersections
//     } else {
//         *t0 = (-b - sqrt(discriminant)) / (2 * a);
//         *t1 = (-b + sqrt(discriminant)) / (2 * a);
//         return 2;  // Two intersections
//     }
// }

// int intersect(t_sphere sphere, t_ray ray, float *t0, float *t1)
// {
//     t_tuple oc = tuple_subtract(ray.origin, sphere.center);
//     float a = dot(ray.direction, ray.direction);
//     float b = 2.0 * dot(oc, ray.direction);
//     float c = dot(oc, oc) - sphere.radius * sphere.radius;
//     float discriminant = b * b - 4 * a * c;

//     if (discriminant < 0) {
//         return 0; // No intersection
//     } else {
//         float sqrt_discriminant = sqrt(discriminant);
//         float root1 = (-b - sqrt_discriminant) / (2 * a);
//         float root2 = (-b + sqrt_discriminant) / (2 * a);

//         if (root1 > root2) {
//             float temp = root1;
//             root1 = root2;
//             root2 = temp;
//         }

//         if (root1 >= 0) {
//             *t0 = root1;
//             if (root2 >= 0) {
//                 *t1 = root2;
//                 return 2; // Two intersections
//             }
//             *t1 = *t0; // Only one intersection is valid and ahead
//             return 1;
//         }
//         if (root2 >= 0) {
//             *t0 = *t1 = root2; // Only the second intersection is valid and ahead
//             return 1;
//         }
//         return 0; // Both intersections are behind
//     }
// }

// t_sphere sphere_create()
// {
//     t_sphere s;
//     s.center = point(0, 0, 0); // Center at world origin
//     s.radius = 1.0; // Unit sphere
//     s.color = t_color_create(1,0,0);
//     return s;
// }

t_sphere sphere_create(t_tuple center, float radius, t_color col)
{
    t_sphere sphere;

    //hittable_init(&sphere.base, sphere_hit);
    sphere.mat.color = col;
    sphere.center = center;
    sphere.radius = fmax(0, radius);

     // Initialize transformation matrices
    sphere.translation_matrix = translation(center.x, center.y, center.z);
    sphere.rotation_matrix = identity_matrix(); // No rotation initially
    sphere.scaling_matrix = scaling(radius, radius, radius);

    // Combine transformations into one matrix
    sphere.transform = t_matrix_multiply(t_matrix_multiply(sphere.translation_matrix, sphere.rotation_matrix), sphere.scaling_matrix);

    // Calculate the inverse transform for ray-sphere intersection
    sphere.inverse_transform = inverse(sphere.transform);
    return (sphere);
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

void test_position_function() {
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


// void printimage(void *param, t_map *map)
// {
// 	t_var *var;
//  	var = param;
// 	(void)map;
//     t_sphere s = sphere_create();
//     t_tuple camera_position = point(0, 0, 0);  // Assuming the camera is at (0,0,0)
//     t_ray r = ray(camera_position, normalize(tuple_subtract(position, camera_position)));

// 	for (int y = 0; y < (int)var->image_height; y++)
// 	{
// 		for (int x = 0; x < SCREEN_WIDTH; x++)
// 		{
//             float t0, t1;
//             t_tuple _ray_origin = point((float)x, (float)y, 0);
//             t_tuple position = point((float)x, (float)y, -10);
//             t_ray r = ray(_ray_origin,normalize(tuple_subtract(position,_ray_origin)));
//             int xs = intersect(s, r ,&t0 , &t1);
//             if (xs > 0) {
//             // Intersection, color the pixel (e.g., red)
//                 write_color(t_color_create(255, 0, 0), var, y, x);
//             } else {
//                 // No intersection, leave it black
//                 write_color(t_color_create(0, 0, 0), var, y, x);
//             }
// 		}
// 	}
//     // test_no_intersection();
//     // test_tangent_intersection();
//     // test_ray_origin_inside_sphere();
//     // test_sphere_behind_ray();
//     // test_sphere_at_origin();
//     //test_position_function();
// 	//print_clock_face(var);
// }

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

    return transform;
}

// Function to compare two tuples (for test validation)
bool tuple_equal(t_tuple t1, t_tuple t2)
{
    return equal(t1.x, t2.x) && equal(t1.y, t2.y) && equal(t1.z, t2.z) && equal(t1.w, t2.w);
}
/*
// Function to check if two tuples (points or vectors) are equal
int tuple_equal(t_tuple *a, t_tuple *b) {
    const float epsilon = 0.00001f;
    return (fabs(a->x - b->x) < epsilon &&
            fabs(a->y - b->y) < epsilon &&
            fabs(a->z - b->z) < epsilon &&
            fabs(a->w - b->w) < epsilon);
}*/

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
t_matrix *translation(float x, float y, float z) {
    // Initialize the identity matrix
    t_matrix *transform = identity_matrix();

    // Set the translation components
    transform->data[0][3] = x;
    transform->data[1][3] = y;
    transform->data[2][3] = z;

    return (transform);
}

t_matrix *inverse(t_matrix *m) {
    // Calculate the determinant first
    float det = determinant(m);
    if (det == 0.0f) {
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

    for (int i = 0; i < inverse_m->rows; i++) {
        for (int j = 0; j < inverse_m->cols; j++) {
            inverse_m->data[i][j] = adjugate_m->data[i][j] / det;
        }
    }

    // Free allocated memory for cofactor and adjugate matrices
    free(cofactor_m);
    free(adjugate_m);

    return inverse_m;
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

    return cofactor_m;
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

    return transposed;
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

    return result;
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
    return m;
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
    return m;
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

t_tuple reflect(t_tuple t1, t_tuple t2)
{
    t1 =  negate_tuple(t1);
    return (tuple_add(t1,t2)); // varmasti paskaa!!

}

//t_color lighting(t_material material, t_point_light light, t_tuple point, t_tuple eyev, t_tuple normalv)
t_color lighting(t_material material, t_point_light light, t_tuple point, t_tuple eyev, t_tuple normalv)
{
    // Combine the surface color with the light's color/intensity
    t_color effective_color = color_multiply_scalar(material.color, light.intensity);

    // Find the direction to the light source
    t_tuple lightv = normalize(tuple_subtract(light.position, point));

    // Compute the ambient contribution
    t_color ambient = color_multiply_scalar(effective_color, material.ambient);

    // Calculate the dot product of the light vector and the normal vector
    float light_dot_normal = dot(lightv, normalv);

    t_color diffuse;
    t_color specular;

    if (light_dot_normal < 0) {
        // The light is on the other side of the surface, no diffuse or specular contribution
        diffuse = t_color_create(0, 0, 0);
        specular = t_color_create(0, 0, 0);
    } else {
        // Compute the diffuse contribution
        diffuse = color_multiply_scalar(effective_color, material.diffuse * light_dot_normal);

        // Reflect the light vector around the normal
        t_tuple reflectv = reflect(negate_tuple(lightv), normalv);

        // Compute reflect_dot_eye (the cosine of the angle between reflection and view vectors)
        float reflect_dot_eye = dot(reflectv, eyev);

        if (reflect_dot_eye <= 0) {
            // The light reflects away from the eye, no specular contribution
            specular = t_color_create(0, 0, 0);
        } else {
            // Compute the specular contribution
            float factor = pow(reflect_dot_eye, material.shininess);
            t_color light_intensity_as_color = t_color_create(light.intensity, light.intensity, light.intensity);
            specular = color_multiply_scalar(light_intensity_as_color, material.specular * factor);
        }
    }

    // Add the ambient, diffuse, and specular contributions
    return color_add(color_add(ambient, diffuse), specular);
}
/*
# combine the surface color with the light's color/intensity
effective_color ← material.color * light.intensity
# find the direction to the light source
lightv ← normalize(light.position - point)
# compute the ambient contribution
ambient ← effective_color * material.ambient
# light_dot_normal represents the cosine of the angle between the
# light vector and the normal vector. A negative number means the
# light is on the other side of the surface.
light_dot_normal ← dot(lightv, normalv)
if light_dot_normal < 0
diffuse ← black
specular ← black
else
# compute the diffuse contribution

diffuse ← effective_color * material.diffuse * light_dot_normal
# reflect_dot_eye represents the cosine of the angle between the
# reflection vector and the eye vector. A negative number means the
# light reflects away from the eye.
reflectv ← reflect(-lightv, normalv)
reflect_dot_eye ← dot(reflectv, eyev)
report erratum • discussPutting It Together • 89
if reflect_dot_eye <= 0
specular ← black
else
# compute the specular contribution
factor ← pow(reflect_dot_eye, material.shininess)
specular ← light.intensity * material.specular * factor
end if
end if
# Add the three contributions together to get the final shading
return ambient + diffuse + specular
*/
t_tuple ray_position(t_ray r, float t)
{
    // Position along the ray at distance t: origin + t * direction
    return tuple_add(r.origin, tuple_multiply(r.direction, t));
}

t_tuple sphere_normal_at(t_sphere s, t_tuple point) {
    // The normal is the vector from the sphere's center to the surface point
    t_tuple object_normal = tuple_subtract(point, s.center);

    // Normalize the result to get the unit normal vector
    return normalize(object_normal);
}

void printimage(void *param, t_map *map)
{
    t_var *var = (t_var *)param;
    t_point_light light;  // Define the light source (position, intensity, etc.)
    //t_material material;  // Define the material properties (ambient, diffuse, specular, etc.)

    light.col = t_color_create(map->light->r, map->light->b, map->light->g);
    light.intensity = map->light->ratio;
    light.position = point(map->light->x, map->light->y, map->light->z);


    // Loop over each pixel on the image
    for (int y = 0; y < (int)var->image_height; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            // Calculate u, v for mapping pixel coordinates to screen space
            float u = (float)x / (float)(SCREEN_WIDTH - 1);
            float v = (float)y / (float)(var->image_height - 1);

            // Compute ray direction from camera for each pixel
            t_tuple ray_direction = normalize(tuple_subtract(
                tuple_add(var->cam.lower_left_corner, tuple_add(
                    tuple_multiply(var->cam.horizontal, u),
                    tuple_multiply(var->cam.vertical, v))),
                var->cam.position));
            t_ray r = ray(var->cam.position, ray_direction);

            // Initialize intersection variables
            float t0, t1;
            t_color final_color = var->ambientl;
            bool hit_anything = false;

            // Loop through all spheres to check for intersections
            for (int i = 0; i < map->element_count->sphere; i++) {
                int xs = intersect(var->test_sphere[i], r, &t0, &t1);

                if (xs > 0) {  // If an intersection occurs
                    hit_anything = true;

                    // Calculate point of intersection and normal at that point
                    t_tuple hit_point = ray_position(r, t0);  // Get the intersection point
                    t_tuple normalv = sphere_normal_at(var->test_sphere[i], hit_point);  // Normal at the intersection
                    t_tuple eyev = negate_tuple(r.direction);  // The view direction (eye vector)

                    // Calculate the lighting at the intersection point
                    t_color lighting_result = lighting(var->test_sphere[i].mat, light, hit_point, eyev, normalv);

                    // Mix ambient, diffuse, and specular contributions
                    final_color = lighting_result;
                    break;  // Stop after the first hit (you could check for the closest hit instead)
                }
            }

            // Write the computed color to the image
            if (hit_anything)
            {
                //final_color = multiply_colors(final_color, var->ambientl);
                write_color(final_color, var, x, y);  // Write the lit color
            }
            else
            {
                write_color(var->ambientl, var, x, y);  // Write ambient color for no hit
            }
        }
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
    init_test_sphere(&var, map); // TESTI SPHERE!!!!!
	printimage(&var, map);
	hooks(&var);
	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	terminate_data(map, "program ended successfully\n");
	return (EXIT_SUCCESS);
}
