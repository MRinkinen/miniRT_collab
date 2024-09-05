/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 10:45:46 by tvalimak          #+#    #+#             */
/*   Updated: 2024/09/05 19:11:11 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Left in the page 53 chaining part

#include "../includes/test_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define EPSILON 0.00001

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
    return equal(c1.red, c2.red) && equal(c1.green, c2.green) && equal(c1.blue, c2.blue);
}

// Function to add two colors
t_color add_colors(t_color c1, t_color c2)
{
    return (t_color){c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
}

// Function to subtract two colors
t_color subtract_colors(t_color c1, t_color c2)
{
    return (t_color){c1.red - c2.red, c1.green - c2.green, c1.blue - c2.blue};
}

// Function to multiply a color by a scalar
t_color multiply_color_scalar(t_color c, double scalar)
{
    return (t_color){c.red * scalar, c.green * scalar, c.blue * scalar};
}

// Function to multiply two colors (Hadamard product)
t_color multiply_colors(t_color c1, t_color c2)
{
    return (t_color){c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
}

// Function to print a color (for debugging)
void print_color(t_color c)
{
    printf("(%.2f, %.2f, %.2f)\n", c.red, c.green, c.blue);
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

// Function to multiply a tuple by a scalar
t_tuple tuple_multiply(t_tuple t, double scalar)
{
    return tuple(t.x * scalar, t.y * scalar, t.z * scalar, t.w * scalar);
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

// Test scenarios
void test_scenarios()
{
    // Scenario: A tuple with w=1.0 is a point
    t_tuple a = tuple(4.3, -4.2, 3.1, 1.0);
    printf("a.x = %.1f\n", a.x);  // Expected output: 4.3
    printf("a.y = %.1f\n", a.y);  // Expected output: -4.2
    printf("a.z = %.1f\n", a.z);  // Expected output: 3.1
    printf("a.w = %.1f\n", a.w);  // Expected output: 1.0
    printf("a is a point: %s\n", is_point(a) ? "true" : "false"); // Expected output: true
    printf("a is a vector: %s\n", is_vector(a) ? "true" : "false"); // Expected output: false

    // Scenario: A tuple with w=0 is a vector
    a = tuple(4.3, -4.2, 3.1, 0.0);
    printf("a.x = %.1f\n", a.x);  // Expected output: 4.3
    printf("a.y = %.1f\n", a.y);  // Expected output: -4.2
    printf("a.z = %.1f\n", a.z);  // Expected output: 3.1
    printf("a.w = %.1f\n", a.w);  // Expected output: 0.0
    printf("a is a point: %s\n", is_point(a) ? "true" : "false"); // Expected output: false
    printf("a is a vector: %s\n", is_vector(a) ? "true" : "false"); // Expected output: true

    // Scenario: point() creates tuples with w=1
    t_tuple p = point(4, -4, 3);
    t_tuple expected_point = tuple(4, -4, 3, 1.0);
    printf("p is correct: %s\n", tuple_equal(p, expected_point) ? "true" : "false"); // Expected output: true

    // Scenario: vector() creates tuples with w=0
    t_tuple v = vector(4, -4, 3);
    t_tuple expected_vector = tuple(4, -4, 3, 0.0);
    printf("v is correct: %s\n", tuple_equal(v, expected_vector) ? "true" : "false"); // Expected output: true

    // Scenario: Adding two tuples
    t_tuple a1 = tuple(3, -2, 5, 1);
    t_tuple a2 = tuple(-2, 3, 1, 0);
    t_tuple result = tuple_add(a1, a2);
    t_tuple expected = tuple(1, 1, 6, 1);
    printf("a1 + a2 = tuple(1, 1, 6, 1): %s\n", tuple_equal(result, expected) ? "true" : "false"); // Expected output: true

    // Scenario: Subtracting two points
    t_tuple p1 = point(3, 2, 1);
    t_tuple p2 = point(5, 6, 7);
    t_tuple result_subtract = tuple_subtract(p1, p2);
    t_tuple expected_subtract = vector(-2, -4, -6);
    printf("p1 - p2 = vector(-2, -4, -6): %s\n", tuple_equal(result_subtract, expected_subtract) ? "true" : "false"); // Expected output: true

    // Scenario: Subtracting a vector from a point
    t_tuple p3 = point(3, 2, 1);
    t_tuple v1 = vector(5, 6, 7);
    t_tuple result_point_subtract_vector = subtract_vector_from_point(p3, v1);
    t_tuple expected_point1 = point(-2, -4, -6);
    printf("p3 - v1 = point(-2, -4, -6): %s\n", tuple_equal(result_point_subtract_vector, expected_point1) ? "true" : "false"); // Expected output: true

    // Scenario: Subtracting two vectors
    t_tuple v2 = vector(3, 2, 1);
    t_tuple v3 = vector(5, 6, 7);
    t_tuple result_subtract_vectors = tuple_subtract(v2, v3);
    t_tuple expected_vector_subtract = vector(-2, -4, -6);
    printf("v2 - v3 = vector(-2, -4, -6): %s\n", tuple_equal(result_subtract_vectors, expected_vector_subtract) ? "true" : "false"); // Expected output: true

    // Scenario: Negating a vector (subtracting it from zero vector)
    t_tuple v4 = vector(1, -2, 3);
    t_tuple result_negate_vector = negate_vector(v4);
    t_tuple expected_negated_vector = vector(-1, 2, -3);
    printf("zero - v4 = vector(-1, 2, -3): %s\n", tuple_equal(result_negate_vector, expected_negated_vector) ? "true" : "false"); // Expected output: true

    // Scenario: Negating a tuple
    t_tuple p4 = tuple(1, -2, 3, -4);
    t_tuple expected_negated_p4 = tuple(-1, 2, -3, 4);
    t_tuple negated_p4 = negate_tuple(p4);
    printf("Negating a tuple (1, -2, 3, -4): %s\n", tuple_equal(negated_p4, expected_negated_p4) ? "true" : "false"); // Expected output: true

    // Scenario: Multiplying a tuple by a scalar
    t_tuple a3 = tuple(1, -2, 3, -4);
    t_tuple result_multiply_scalar = tuple_multiply(a3, 3.5);
    t_tuple expected_multiply_scalar = tuple(3.5, -7, 10.5, -14);
    printf("a * 3.5 = tuple(3.5, -7, 10.5, -14): %s\n", tuple_equal(result_multiply_scalar, expected_multiply_scalar) ? "true" : "false"); // Expected output: true

    // Scenario: Multiplying a tuple by a fraction
    t_tuple result_multiply_fraction = tuple_multiply(a3, 0.5);
    t_tuple expected_multiply_fraction = tuple(0.5, -1, 1.5, -2);
    printf("a * 0.5 = tuple(0.5, -1, 1.5, -2): %s\n", tuple_equal(result_multiply_fraction, expected_multiply_fraction) ? "true" : "false"); // Expected output: true

    // Scenario: Dividing a tuple by a scalar
    t_tuple result_divide_scalar = tuple_divide(a3, 2);
    t_tuple expected_divide_scalar = tuple(0.5, -1, 1.5, -2);
    printf("a / 2 = tuple(0.5, -1, 1.5, -2): %s\n", tuple_equal(result_divide_scalar, expected_divide_scalar) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(1, 0, 0)
    t_tuple v5 = vector(1, 0, 0);
    printf("Magnitude of vector(1, 0, 0) = 1: %s\n", magnitude_equal(v5, 1) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(0, 1, 0)
    t_tuple v6 = vector(0, 1, 0);
    printf("Magnitude of vector(0, 1, 0) = 1: %s\n", magnitude_equal(v6, 1) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(0, 0, 1)
    t_tuple v7 = vector(0, 0, 1);
    printf("Magnitude of vector(0, 0, 1) = 1: %s\n", magnitude_equal(v7, 1) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(1, 2, 3)
    t_tuple v8 = vector(1, 2, 3);
    printf("Magnitude of vector(1, 2, 3) = √14: %s\n", magnitude_equal(v8, sqrt(14)) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(-1, -2, -3)
    t_tuple v9 = vector(-1, -2, -3);
    printf("Magnitude of vector(-1, -2, -3) = √14: %s\n", magnitude_equal(v9, sqrt(14)) ? "true" : "false"); // Expected output: true

    // Scenario: Normalizing vector(4, 0, 0)
    t_tuple v10 = vector(4, 0, 0);
    t_tuple result_normalize_v10 = normalize(v10);
    t_tuple expected_normalize_v10 = vector(1, 0, 0);
    printf("Normalizing vector(4, 0, 0): %s\n", tuple_equal(result_normalize_v10, expected_normalize_v10) ? "true" : "false"); // Expected output: true

    // Scenario: Normalizing vector(1, 2, 3)
    t_tuple v11 = vector(1, 2, 3);
    t_tuple result_normalize_v11 = normalize(v11);
    // Vector(1/√14, 2/√14, 3/√14)
    t_tuple expected_normalize_v11 = vector(0.26726, 0.53452, 0.80178);
    printf("Normalizing vector(1, 2, 3): %s\n", tuple_equal(result_normalize_v11, expected_normalize_v11) ? "true" : "false"); // Expected output: true

    // Scenario: The magnitude of a normalized vector
    t_tuple norm = normalize(v11);
    printf("Magnitude of normalized vector(1, 2, 3): %s\n", equal(magnitude(norm), 1) ? "true" : "false"); // Expected output: true

    // Scenario: The dot product of two vectors
    t_tuple v12 = vector(1, 2, 3);
    t_tuple v13 = vector(2, 3, 4);
    double result_dot = dot(v12, v13);
    double expected_dot = 1*2 + 2*3 + 3*4; // 2 + 6 + 12 = 20
    printf("Dot product of vector(1, 2, 3) and vector(2, 3, 4): %s\n", equal(result_dot, expected_dot) ? "true" : "false"); // Expected output: true

    // Scenario: The cross product of two vectors
    t_tuple v14 = vector(1, 2, 3);
    t_tuple v15 = vector(2, 3, 4);

    t_tuple result_cross_v14_v15 = cross(v14, v15);
    t_tuple expected_cross_v14_v15 = vector(-1, 2, -1);
    printf("Cross product of vector(1, 2, 3) and vector(2, 3, 4): %s\n", tuple_equal(result_cross_v14_v15, expected_cross_v14_v15) ? "true" : "false"); // Expected output: true

    t_tuple result_cross_v15_v14 = cross(v15, v14);
    t_tuple expected_cross_v15_v14 = vector(1, -2, 1);
    printf("Cross product of vector(2, 3, 4) and vector(1, 2, 3): %s\n", tuple_equal(result_cross_v15_v14, expected_cross_v15_v14) ? "true" : "false"); // Expected output: true

    // Scenario: Adding colors
    t_color c1 = {0.9, 0.6, 0.75};
    t_color c2 = {0.7, 0.1, 0.25};
    t_color result_add = add_colors(c1, c2);
    t_color expected_add = {1.6, 0.7, 1.0};
    printf("Adding colors: ");
    print_color(result_add);
    printf("Test %s\n", compare_colors(result_add, expected_add) ? "Passed" : "Failed");

    // Scenario: Subtracting colors
    t_color color_result_subtract = subtract_colors(c1, c2);
    t_color color_expected_subtract = {0.2, 0.5, 0.5};
    printf("Subtracting colors: ");
    print_color(color_result_subtract);
    printf("Test %s\n", compare_colors(color_result_subtract, color_expected_subtract) ? "Passed" : "Failed");

    // Scenario: Multiplying a color by a scalar
    t_color c3 = {0.2, 0.3, 0.4};
    t_color color_result_multiply_scalar = multiply_color_scalar(c3, 2);
    t_color color_expected_multiply_scalar = {0.4, 0.6, 0.8};
    printf("Multiplying color by scalar: ");
    print_color(color_result_multiply_scalar);
    printf("Test %s\n", compare_colors(color_result_multiply_scalar, color_expected_multiply_scalar) ? "Passed" : "Failed");

    // Scenario: Multiplying colors (Hadamard product)
    t_color c4 = {1, 0.2, 0.4};
    t_color c5 = {0.9, 1, 0.1};
    t_color result_multiply_colors = multiply_colors(c4, c5);
    t_color expected_multiply_colors = {0.9, 0.2, 0.04};
    printf("Multiplying colors: ");
    print_color(result_multiply_colors);
    printf("Test %s\n", compare_colors(result_multiply_colors, expected_multiply_colors) ? "Passed" : "Failed");

    t_color c6 = {1.0, 0.2, 0.4};
    t_color c7 = {0.9, 1.0, 0.1};

    t_color blended_color = hadamard_product(c6, c7);

    printf("Blended t_color: ");
    print_color(blended_color);

     t_matrix *A = create_4x4_matrix(1, 2, 3, 4,
                                    5, 6, 7, 8,
                                    9, 8, 7, 6,
                                    5, 4, 3, 2);

    t_matrix *B = create_4x4_matrix(1, 2, 3, 4,
                                    5, 6, 7, 8,
                                    9, 8, 7, 6,
                                    5, 4, 3, 2);

    t_matrix *C = create_4x4_matrix(2, 3, 4, 5,
                                    6, 7, 8, 9,
                                    8, 7, 6, 5,
                                    4, 3, 2, 1);

    if (t_matrix_equal(A, B))
    {
        printf("Matrix A is equal to Matrix B\n");
    }
    else
    {
        printf("Matrix A is not equal to Matrix B\n");
    }

    if (t_matrix_equal(A, C))
    {
        printf("Matrix A is equal to Matrix C\n");
    }
    else
    {
        printf("Matrix A is not equal to Matrix C\n");
    }

    // Free allocated memory for matrices
    free(A);
    free(B);
    free(C);

   // Define matrix D
    t_matrix *D = create_4x4_matrix(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    );

    // Define matrix E
    t_matrix *E = create_4x4_matrix(
        -2, 1, 2, 3,
        3, 2, 1, -1,
        4, 3, 6, 5,
        1, 2, 7, 8
    );

    // Multiply D and E
    t_matrix *F = t_matrix_multiply(D, E);

    // Print the result
    if (F != NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%6.1f ", F->data[i][j]);
            }
            printf("\n");
        }
    }

    // Free allocated memory
    free(D);
    free(E);
    free(F);

     t_matrix *G = create_4x4_matrix(
        0, 1, 2, 4,
        1, 2, 4, 8,
        2, 4, 8, 16,
        4, 8, 16, 32
    );

    // Create the identity matrix
    t_matrix *I = identity_matrix();

    // Multiply A with the identity matrix
    t_matrix *multiply_result = t_matrix_multiply(G, I);

    // Print the result, which should be the same as A
    if (multiply_result != NULL)
    {
        int i = 0;
        while (i < 4)
        {
            int j = 0;
            while (j < 4)
            {
                printf("%6.1f ", multiply_result->data[i][j]);
                j++;
            }
            printf("\n");
            i++;
        }
    }

    // Free allocated memory
    free(G);
    free(I);
    free(multiply_result);

    // Define the matrix A
    t_matrix *H = create_4x4_matrix(
        0, 9, 3, 0,
        9, 8, 0, 8,
        1, 8, 5, 3,
        0, 0, 5, 8
    );

    // Transpose matrix A
    t_matrix *transposed_H = transpose(H);

    // Print the transposed matrix
    if (transposed_H != NULL)
    {
        int i = 0;
        while (i < transposed_H->rows)
        {
            int j = 0;
            while (j < transposed_H->cols)
            {
                printf("%6.1f ", transposed_H->data[i][j]);
                j++;
            }
            printf("\n");
            i++;
        }
    }

    // Check transposing identity matrix
    t_matrix *identity = identity_matrix();
    t_matrix *transposed_identity = transpose(identity);

    // Compare the transposed identity matrix to the original identity matrix
    if (t_matrix_equal(identity, transposed_identity))
    {
        printf("Transpose of identity matrix is equal to the identity matrix.\n");
    }
    else
    {
        printf("Transpose of identity matrix is NOT equal to the identity matrix.\n");
    }

    // Free allocated memory
    free(H);
    free(transposed_H);
    free(identity);
    free(transposed_identity);

    // Define the 2x2 matrix A
    t_matrix *J = create_2x2_matrix(1, 5, -3, 2);

    // Calculate the determinant of matrix A
    float det = determinant_2x2(J);

    // Print the determinant
    printf("Determinant of matrix A: %f\n", det);

    // Check if the determinant matches the expected value
    if (det == 17.0)
    {
        printf("Test passed: Determinant is correct.\n");
    }
    else
    {
        printf("Test failed: Determinant is incorrect.\n");
    }

    // Free allocated memory
    free(J);

    // Scenario 1: A submatrix of a 3x3 matrix is a 2x2 matrix
    t_matrix *A_3x3 = create_3x3_matrix(1, 5, 0, -3, 2, 7, 0, 6, -3);

    // Extract submatrix by removing row 0 and column 2
    t_matrix *sub_A_3x3 = submatrix(A_3x3, 0, 2);

    printf("Submatrix of A_3x3 (after removing row 0 and col 2):\n");
    for (int i = 0; i < sub_A_3x3->rows; i++)
    {
        for (int j = 0; j < sub_A_3x3->cols; j++)
        {
            printf("%f ", sub_A_3x3->data[i][j]);
        }
        printf("\n");
    }

    // Scenario 2: A submatrix of a 4x4 matrix is a 3x3 matrix
    t_matrix *A_4x4 = create_4x4_matrix(-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1);

    // Extract submatrix by removing row 2 and column 1
    t_matrix *sub_A_4x4 = submatrix(A_4x4, 2, 1);

    printf("Submatrix of A_4x4 (after removing row 2 and col 1):\n");
    for (int i = 0; i < sub_A_4x4->rows; i++)
    {
        for (int j = 0; j < sub_A_4x4->cols; j++)
        {
            printf("%f ", sub_A_4x4->data[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory
    free(A_3x3);
    free(sub_A_3x3);
    free(A_4x4);
    free(sub_A_4x4);

    // Create a 3x3 matrix
    t_matrix *K = create_3x3_matrix(3, 5, 0,
                                2, -1, -7,
                                6, -1, 5);

    // Calculate the minor at position (1, 0)
    float minor_value = minor(K, 1, 0);

    printf("Minor(K, 1, 0) = %f\n", minor_value); // Should print 25

    // Free the matrix memory
    free(K);

    // Create the 3x3 matrix L
    t_matrix *L = create_3x3_matrix(3, 5, 0,
                                    2, -1, -7,
                                    6, -1, 5);

    // Calculate the minor and cofactor at position (0, 0)
    float minor_00 = minor(L, 0, 0);
    float cofactor_00 = cofactor(L, 0, 0);

    printf("Minor(L, 0, 0) = %f\n", minor_00);       // Should print -12
    printf("Cofactor(L, 0, 0) = %f\n", cofactor_00); // Should print -12

    // Calculate the minor and cofactor at position (1, 0)
    float minor_10 = minor(L, 1, 0);
    float cofactor_10 = cofactor(L, 1, 0);

    printf("Minor(L, 1, 0) = %f\n", minor_10);       // Should print 25
    printf("Cofactor(L, 1, 0) = %f\n", cofactor_10); // Should print -25

    // Free the matrix memory
    free(L);

    // Invertible matrix example
    t_matrix *M = create_4x4_matrix(6, 4, 4, 4,
                                    5, 5, 7, 6,
                                    4, -9, 3, -7,
                                    9, 1, 7, -6);

    float det_M = determinant(M);
    bool invertible_M = is_invertible(M);

    printf("Determinant(M) = %f\n", det_M);  // Should print -2120
    printf("M is invertible: %s\n", invertible_M ? "true" : "false");  // Should print true
    // Non-invertible matrix example
    t_matrix *N = create_4x4_matrix(-4, 2, -2, -3,
                                    9, 6, 2, 6,
                                    0, -5, 1, -5,
                                    0, 0, 0, 0);

    float det_N = determinant(N);
    bool invertible_N = is_invertible(N);
    printf("Determinant(N) = %f\n", det_N);  // Should print 0
    printf("N is invertible: %s\n", invertible_N ? "true" : "false");  // Should print false
    // Free the matrices after use
    free(M);
    free(N);

    // Test case for 2x2 matrix
    t_matrix m2x2 = { .rows = 2, .cols = 2, .data = { {1, 2}, {3, 4} } };
    printf("Determinant of 2x2 matrix: %f\n", determinant(&m2x2)); // Should print -2

    // Test case for 3x3 matrix
    t_matrix m3x3 = { .rows = 3, .cols = 3, .data = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} } };
    printf("Determinant of 3x3 matrix: %f\n", determinant(&m3x3)); // Should print 0

    // Create a 4x4 matrix
    t_matrix m4x4 = {
        .rows = 4,
        .cols = 4,
        .data = {
            {6, 4, 4, 4},
            {5, 5, 7, 6},
            {4, -9, 3, -7},
            {9, 1, 7, -6}
        }
    };

    // Calculate the determinant
    float det_m4x4 = determinant(&m4x4);
    printf("Determinant(M) = %f\n", det_m4x4); // Should print -2120

    t_matrix O = {
        .rows = 4, .cols = 4,
        .data = {
            {-5, 2, 6, -8},
            {1, -5, 1, 8},
            {7, 7, -6, -7},
            {1, -3, 7, 4}
        }
    };

    // Calculate the inverse of P
    t_matrix *P = inverse(&O);

    // Print the determinant, cofactor, and elements of the inverse matrix
    printf("Determinant(O) = %f\n", determinant(&O));  // Should print 532

    printf("Cofactor(O, 2, 3) = %f\n", cofactor(&O, 2, 3));  // Should print -160
    printf("Cofactor(O, 3, 2) = %f\n", cofactor(&O, 3, 2));  // Should print 105

    printf("P[3,2] = %f\n", P->data[3][2]);  // Should print -160/532
    printf("P[2,3] = %f\n", P->data[2][3]);  // Should print 105/532

    // Print the full inverse matrix O
    printf("Inverse Matrix P:\n");
    int i = 0;
    while (i < O.rows) {
        int j = 0;
        while (j < O.cols) {
            printf("%f ", O.data[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }

    t_matrix *A1 = create_4x4_matrix(8, -5, 9, 2,
                                    7, 5, 6, 1,
                                    -6, 0, 9, 6,
                                    -3, 0, -9, -4);

    t_matrix *expected_inv_A1 = create_4x4_matrix(-0.15385, -0.15385, -0.28205, -0.53846,
                                                 -0.07692,  0.12308,  0.02564,  0.03077,
                                                  0.35897,  0.35897,  0.43590,  0.92308,
                                                 -0.69231, -0.69231, -0.76923, -1.92308);

    t_matrix *inv_A1 = inverse(A1);

    if (t_matrix_equal(inv_A1, expected_inv_A1) == 1)
    {
        printf("Test Case 1 Passed!\n");
    }
    else
    {
        printf("Test Case 1 Failed!\n");
    }
    // Test Case 2
    t_matrix *A2 = create_4x4_matrix(9, 3, 0, 9,
                                    -5, -2, -6, -3,
                                    -4, 9, 6, 4,
                                    -7, 6, 6, 2);

    t_matrix *expected_inv_A2 = create_4x4_matrix(-0.04074, -0.07778,  0.14444, -0.22222,
                                                 -0.07778,  0.03333,  0.36667, -0.33333,
                                                 -0.02901, -0.14630, -0.10926,  0.12963,
                                                  0.17778,  0.06667, -0.26667,  0.33333);

    t_matrix *inv_A2 = inverse(A2);

    if (t_matrix_equal(inv_A2, expected_inv_A2)) {
        printf("Test Case 2 Passed!\n");
    } else {
        printf("Test Case 2 Failed!\n");
    }
    // Create a translation matrix
    t_matrix *transform = translation(5, -3, 2);
    // Define a point
    t_tuple p5 = point(-3, 4, 5);
    // Multiply the translation matrix by the point
    t_tuple result1 = multiply_matrix_tuple(transform, &p5);
    // Expected result after translation
    t_tuple expected1 = point(2, 1, 7);
    // Check if the result matches the expected point
    if (equal(result1.x, expected1.x) &&
        equal(result1.y, expected1.y) &&
        equal(result1.z, expected1.z))
    {
        printf("Test for multiplying by a translation matrix Passed!\n");
    }
    else
    {
        printf("Test for multiplying by a translation matrix Failed!\n");
    }

    // Given: transform ← translation(5, -3, 2)
    t_matrix *transform1 = translation(5, -3, 2);

    // And: inv ← inverse(transform)
    t_matrix *inv = inverse_translation(transform1);

    // And: p ← point(-3, 4, 5)
    t_tuple point1 = point(-3, 4, 5);

    // Then: inv * p = point(-8, 7, 3)
    t_tuple expected2 = point(-8, 7, 3);
    t_tuple result2 = multiply_matrix_tuple(inv, &point1);

    if (tuple_equal(result2, expected2)) {
        printf("Test Passed: inv * p = point(-8, 7, 3)\n");
    } else {
        printf("Test Failed: inv * p != point(-8, 7, 3)\n");
        printf("Result: (%f, %f, %f)\n", result.x, result.y, result.z);
    }

    // Free allocated memory
    free(transform1);
    free(inv);

    // Given: transform ← translation(5, -3, 2)
    t_matrix *transform2 = translation(5, -3, 2);

    // And: v ← vector(-3, 4, 5)
    t_tuple v16 = vector(-3, 4, 5);

    // Then: transform * v = v
    t_tuple result3 = multiply_matrix_tuple(transform2, &v16);

    if (tuple_equal(result3, v16)) {
        printf("Test Passed: transform * v = v\n");
    } else {
        printf("Test Failed: transform * v != v\n");
        printf("Result: (%f, %f, %f)\n", result.x, result.y, result.z);
    }

    // Free allocated memory
    free(transform2);

    // Given: transform ← scaling(2, 3, 4)
    t_matrix *transform3 = scaling(2, 3, 4);

    // And: p ← point(-4, 6, 8)
    t_tuple point2 = point(-4, 6, 8);

    // Then: transform * p = point(-8, 18, 32)
    t_tuple expected3 = point(-8, 18, 32);
    t_tuple result4 = multiply_matrix_tuple(transform3, &point2);

    if (tuple_equal(result4, expected3)) {
        printf("Test Passed: transform * p = point(-8, 18, 32)\n");
    } else {
        printf("Test Failed: transform * p != point(-8, 18, 32)\n");
        printf("Result: (%f, %f, %f)\n", result4.x, result4.y, result4.z);
    }

    // Free allocated memory
    free(transform3);

    // Given: transform ← scaling(2, 3, 4)
    t_matrix *transform4 = scaling(2, 3, 4);

    // And: inv ← inverse(transform)
    t_matrix *inv2 = inverse_scaling(2, 3, 4);

    // And: v ← vector(-4, 6, 8)
    t_tuple v17 = vector(-4, 6, 8);

    // Then: inv * v = vector(-2, 2, 2)
    t_tuple expected4 = vector(-2, 2, 2);
    t_tuple result5 = multiply_matrix_tuple(inv2, &v17);

    if (tuple_equal(result5, expected4)) {
        printf("Test Passed: inv * v = vector(-2, 2, 2)\n");
    } else {
        printf("Test Failed: inv * v != vector(-2, 2, 2)\n");
        printf("Result: (%f, %f, %f)\n", result5.x, result5.y, result5.z);
    }

    // Free allocated memory
    free(transform4);
    free(inv2);

    // Given transform ← scaling(-1, 1, 1)
    t_matrix *transform5 = reflective_scaling(-1, 1, 1);
    // And p ← point(2, 3, 4)
    t_tuple point3 = point(2, 3, 4);
    // Then transform * p = point(-2, 3, 4)
    t_tuple result6 = multiply_matrix_tuple(transform5, &point3);
    t_tuple expected5 = point(-2, 3, 4);
    // Verify that the result matches the expected reflection
    if (tuple_equal(result6, expected5)) {
        printf("Test Passed: Reflection is scaling by a negative value\n");
    } else {
        printf("Test Failed: Reflection is not scaling by a negative value\n");
        printf("Result: (%f, %f, %f)\n", result6.x, result6.y, result6.z);
    }
    // Free allocated memory
    free(transform5);

    // Given p ← point(0, 1, 0)
    t_tuple point4 = point(0, 1, 0);

    // And half_quarter ← rotation_x(π / 4)
    t_matrix *half_quarter = rotation_x(M_PI / 4);

    // And full_quarter ← rotation_x(π / 2)
    t_matrix *full_quarter = rotation_x(M_PI / 2);

    // Then half_quarter * p = point(0, √2/2, √2/2)
    t_tuple result_half_quarter = multiply_matrix_tuple(half_quarter, &point4);
    t_tuple expected_half_quarter = point(0, sqrt(2) / 2, sqrt(2) / 2);

    if (tuple_equal(result_half_quarter, expected_half_quarter))
        printf("Test Passed: Half quarter rotation around X axis\n");
    else
    {
        printf("Test Failed: Half quarter rotation around X axis\n");
        printf("Result: (%f, %f, %f)\n", result_half_quarter.x, result_half_quarter.y, result_half_quarter.z);
    }
    // Then full_quarter * p = point(0, 0, 1)
    t_tuple result_full_quarter = multiply_matrix_tuple(full_quarter, &point4);
    t_tuple expected_full_quarter = point(0, 0, 1);
    if (tuple_equal(result_full_quarter, expected_full_quarter))
    {
        printf("Test Passed: Full quarter rotation around X axis\n");
    } else
    {
        printf("Test Failed: Full quarter rotation around X axis\n");
        printf("Result: (%f, %f, %f)\n", result_full_quarter.x, result_full_quarter.y, result_full_quarter.z);
    }
    // Free allocated memory
    free(half_quarter);
    free(full_quarter);

    t_tuple point5 = point(0, 1, 0);
    t_matrix* half_quarter2 = rotation_x(M_PI / 4);
    t_matrix* inv3 = inverse(half_quarter2);

    t_tuple result7 = multiply_matrix_tuple(inv3, &point5);
    t_tuple expected6 = point(0, sqrt(2)/2, -sqrt(2)/2);

    if (tuple_equal(result7, expected6)) {
        printf("Inverse X-Rotation Test Passed!\n");
    } else {
        printf("Inverse X-Rotation Test Failed!\n");
    }

    t_tuple point6 = point(0, 0, 1);

    // Rotating by π/4 (45 degrees)
    t_matrix* half_quarter3 = rotation_y(M_PI / 4);
    t_tuple result_half_quarter2 = multiply_matrix_tuple(half_quarter3, &point6);
    t_tuple expected_half_quarter2 = point(sqrt(2)/2, 0, sqrt(2)/2);

    if (tuple_equal(result_half_quarter2, expected_half_quarter2))
    {
        printf("Y-Rotation Half Quarter Test Passed!\n");
    } else {
        printf("Y-Rotation Half Quarter Test Failed!\n");
    }
    // Rotating by π/2 (90 degrees)
    t_matrix* full_quarter3 = rotation_y(M_PI / 2);
    t_tuple result_full_quarter3 = multiply_matrix_tuple(full_quarter3, &point6);
    t_tuple expected_full_quarter3 = point(1, 0, 0);

    if (tuple_equal(result_full_quarter3, expected_full_quarter3))
    {
        printf("Y-Rotation Full Quarter Test Passed!\n");
    } else {
        printf("Y-Rotation Full Quarter Test Failed!\n");
    }

    t_tuple point7 = point(0, 1, 0);

    // Rotating by π/4 (45 degrees)
    t_matrix* half_quarter4 = rotation_z(M_PI / 4);
    t_tuple result_half_quarter3 = multiply_matrix_tuple(half_quarter4, &point7);
    t_tuple expected_half_quarter3 = point(-sqrt(2)/2, sqrt(2)/2, 0);

    if (tuple_equal(result_half_quarter3, expected_half_quarter3))
    {
        printf("Z-Rotation Half Quarter Test Passed!\n");
    } else
    {
        printf("Z-Rotation Half Quarter Test Failed!\n");
    }

    // Rotating by π/2 (90 degrees)
    t_matrix* full_quarter4 = rotation_z(M_PI / 2);
    t_tuple result_full_quarter4 = multiply_matrix_tuple(full_quarter4, &point7);
    t_tuple expected_full_quarter4 = point(-1, 0, 0);

    if (tuple_equal(result_full_quarter4, expected_full_quarter4)) {
        printf("Z-Rotation Full Quarter Test Passed!\n");
    } else {
        printf("Z-Rotation Full Quarter Test Failed!\n");
    }

    t_tuple point8 = point(0, 1, 0);

    // Rotating by π/4 (45 degrees)
    t_matrix* half_quarter5 = rotation_z(M_PI / 4);
    t_tuple result_half_quarter4 = multiply_matrix_tuple(half_quarter5, &point8);
    t_tuple expected_half_quarter4 = point(-sqrt(2)/2, sqrt(2)/2, 0);

    if (tuple_equal(result_half_quarter4, expected_half_quarter4))
    {
        printf("Z-Rotation Half Quarter Test Passed!\n");
    } else
    {
        printf("Z-Rotation Half Quarter Test Failed!\n");
    }

    // Rotating by π/2 (90 degrees)
    t_matrix* full_quarter5 = rotation_z(M_PI / 2);
    t_tuple result_full_quarter5 = multiply_matrix_tuple(full_quarter5, &point8);
    t_tuple expected_full_quarter5 = point(-1, 0, 0);

    if (tuple_equal(result_full_quarter5, expected_full_quarter5)) {
        printf("Z-Rotation Full Quarter Test Passed!\n");
    } else {
        printf("Z-Rotation Full Quarter Test Failed!\n");
    }

    // Scenario: Creating and querying a ray
    t_tuple origin = point(1, 2, 3);
    t_tuple direction = vector(4, 5, 6);
    t_ray r = ray(origin, direction);

    printf("t_ray origin: (%f, %f, %f)\n", r.origin.x, r.origin.y, r.origin.z);
    printf("t_ray direction: (%f, %f, %f)\n", r.direction.x, r.direction.y, r.direction.z);

    // Check if the ray's origin and direction match the expected values
    assert(tuple_equal(r.origin, origin));
    assert(tuple_equal(r.direction, direction));

    // Test the position function
    double t = 3.5;
    t_tuple point_on_ray = position(r, t);
    printf("Point on ray at t=%f: (%f, %f, %f)\n", t, point_on_ray.x, point_on_ray.y, point_on_ray.z);

    // Expected point calculation
    t_tuple expected_point = point(1 + 3.5 * 4, 2 + 3.5 * 5, 3 + 3.5 * 6);
    assert(tuple_equal(point_on_ray, expected_point));

    // Scenario: Creating and querying a ray
    t_tuple origin = point(1, 2, 3);
    t_tuple direction = vector(4, 5, 6);
    t_ray r = ray(origin, direction);

    printf("t_ray origin: (%f, %f, %f)\n", r.origin.x, r.origin.y, r.origin.z);
    printf("t_ray direction: (%f, %f, %f)\n", r.direction.x, r.direction.y, r.direction.z);

    // Check if the ray's origin and direction match the expected values
    assert(tuple_equal(r.origin, origin));
    assert(tuple_equal(r.direction, direction));

    // Test the position function
    double t = 3.5;
    t_tuple point_on_ray = position(r, t);
    printf("Point on ray at t=%f: (%f, %f, %f)\n", t, point_on_ray.x, point_on_ray.y, point_on_ray.z);

    // Expected point calculation
    t_tuple expected_point = point(1 + 3.5 * 4, 2 + 3.5 * 5, 3 + 3.5 * 6);
    assert(tuple_equal(point_on_ray, expected_point));

    return ;
}

// Function to concatenate two arrays
int* concatenateArrays(int* a, int size_a, int* b, int size_b, int* size_c)
{
    *size_c = size_a + size_b;
    int i;

    i = 0;
    // Allocate memory for the new array
    int* c = (int*)malloc(*size_c * sizeof(int));
    // Check if memory allocation is successful
    if (c == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    // Copy elements from the first array to the new array
    while (i < size_a)
    {
        c[i] = a[i];
        i++;
    }
    // Copy elements from the second array to the new array
    i = 0;
    for (int i = 0; i < size_b; i++)
    while (i < size_b)
    {
        c[size_a + i] = b[i];
        i++;
    }
    return (c);
}

int main() {
    // Define the two arrays
    int a[] = {1, 2, 3};
    int b[] = {3, 4, 5};

    // Sizes of the arrays
    int size_a = sizeof(a) / sizeof(a[0]);
    int size_b = sizeof(b) / sizeof(b[0]);
    int size_c;

    // Concatenate the arrays
    int* c = concatenateArrays(a, size_a, b, size_b, &size_c);

    // Print the resulting concatenated array
    printf("Concatenated array: ");
    for (int i = 0; i < size_c; i++) {
        printf("%d ", c[i]);
    }
    printf("\n");

    // Free the allocated memory
    free(c);

    test_scenarios();
    return 0;
}
