/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:18 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/14 16:50:45 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// using color = t_vec3;
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void write_color(t_color col, t_var *var, int x, int y)
{
    // Translate the [0,1] component values to the byte range [0,255].
    // int ir = (int)(255.999 * col.r);
    // int ig = (int)(255.999 * col.b);
    // int ib = (int)(255.999 * col.g);
    //printf("R %f G %f B %f \n", col.r, col.b, col.g);
    int color = ft_pixel(col.r, col.b, col.g, 255);
    mlx_put_pixel(var->testimage, x, y, color);
}

void initialize_camera(t_var *var, t_cam *camera, t_map *map)
{
    float aspect_ratio = 16.0f / 9.0f;
    float theta = map->camera->fov * (PI / 180.0f);
    float half_height = tan(theta / 2);
    float half_width = aspect_ratio * half_height;

    t_tuple lookfrom = point(map->camera->x, map->camera->y, map->camera->z);
    t_tuple lookat = point(map->camera->nx, map->camera->ny, map->camera->nz);
    t_tuple vup = point(0.0f, 1.0f, 0.0f);

    printf("FOV: %f degrees, Aspect Ratio: %f\n", map->camera->fov, aspect_ratio);

    var->cam.position = point(map->camera->x, map->camera->y, map->camera->z);
    camera->w = normalize(tuple_subtract(lookfrom, lookat));
    camera->u = normalize(cross(vup, camera->w));
    camera->v = cross(camera->w, camera->u);

    camera->lower_left_corner = tuple_subtract(
        tuple_subtract(
            tuple_subtract(camera->position, tuple_multiply(camera->u, half_width)),
            tuple_multiply(camera->v, half_height)),
        camera->w);
    camera->horizontal = tuple_multiply(camera->u, 2 * half_width);
    camera->vertical = tuple_multiply(camera->v, 2 * half_height);
}

bool check_cap(t_ray ray, float t)
{
    float x = ray.origin.x + t * ray.direction.x;
    float z = ray.origin.z + t * ray.direction.z;
    return (x * x + z * z) <= 1.0f; // Assuming radius is 1 in local space
}

t_matrix* rotation_around_axis(t_tuple axis, float angle)
{
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float cos_theta = cos(angle);
    float sin_theta = sin(angle);
    float one_minus_cos = 1.0f - cos_theta;
    // Prepare the array of values for the matrix
    float values[16] = {
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
    };
    // Create the 4x4 matrix using the values array
    t_matrix* rotation = create_4x4_matrix(values);
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

t_matrix *tuple_to_matrix(t_tuple *t)
{
    // Prepare the array of values for the matrix
    float values[16] = {
        t->x, 0, 0, 0,
        t->y, 0, 0, 0,
        t->z, 0, 0, 0,
        t->w, 0, 0, 0
    };

    // Create the 4x4 matrix using the values array
    t_matrix *m = create_4x4_matrix(values);

    return m;
}
/*
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
}*/

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

t_matrix *create_4x4_matrix(float values[16])
{
    int             row;
    int             col;
    t_matrix    *matrix;

    matrix = (t_matrix *)malloc(sizeof(t_matrix));
    if (matrix == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    matrix->rows = 4;
    matrix->cols = 4;
    row = 0;
    while (row < 4)
    {
        col = 0;
        while (col < 4)
        {
            matrix->data[row][col] = values[row * 4 + col];
            col++;
        }
        row++;
    }
    return (matrix);
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

// Function to compare two colors
bool compare_colors(t_color c1, t_color c2)
{
    return equal(c1.r, c2.r) && equal(c1.g, c2.g) && equal(c1.b, c2.b);
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
float magnitude(t_tuple v)
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
float dot(t_tuple a, t_tuple b)
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

t_tuple ray_at(t_ray ray, float t)
{
    return tuple_add(ray.origin, tuple_multiply(ray.direction, t));
}

// Function to convert degrees to radians
float degrees_to_radians(float degrees) {
    return degrees * (PI / 180.0f);
}

