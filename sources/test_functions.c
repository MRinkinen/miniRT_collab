/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 10:45:46 by tvalimak          #+#    #+#             */
/*   Updated: 2024/09/01 00:48:06 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Left in the page 30

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define EPSILON 0.00001

typedef struct 
{
    int rows;
    int cols;
    float data[4][4]; // This can hold up to a 4x4 matrix, adjust the size if necessary
} t_matrix;

typedef struct 
{
    double x, y, z, w;
}         Tuple;

typedef struct 
{
    double red;
    double green;
    double blue;
} Color;

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
Tuple tuple(double x, double y, double z, double w)
{
    Tuple t;
    t.x = x;
    t.y = y;
    t.z = z;
    t.w = w;
    return (t);
}

Color hadamard_product(Color c1, Color c2) 
{
    Color result;
    result.red = c1.red * c2.red;
    result.green = c1.green * c2.green;
    result.blue = c1.blue * c2.blue;
    return result;
}

// Function to compare two colors
bool compare_colors(Color c1, Color c2) 
{
    return equal(c1.red, c2.red) && equal(c1.green, c2.green) && equal(c1.blue, c2.blue);
}

// Function to add two colors
Color add_colors(Color c1, Color c2) 
{
    return (Color){c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
}

// Function to subtract two colors
Color subtract_colors(Color c1, Color c2) 
{
    return (Color){c1.red - c2.red, c1.green - c2.green, c1.blue - c2.blue};
}

// Function to multiply a color by a scalar
Color multiply_color_scalar(Color c, double scalar) 
{
    return (Color){c.red * scalar, c.green * scalar, c.blue * scalar};
}

// Function to multiply two colors (Hadamard product)
Color multiply_colors(Color c1, Color c2) 
{
    return (Color){c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
}

// Function to print a color (for debugging)
void print_color(Color c) 
{
    printf("(%.2f, %.2f, %.2f)\n", c.red, c.green, c.blue);
}

// Function to create a point (w = 1.0)
Tuple point(double x, double y, double z) 
{
    return tuple(x, y, z, 1.0);
}

// Function to create a vector (w = 0.0)
Tuple vector(double x, double y, double z) 
{
    return tuple(x, y, z, 0.0);
}

// Function to check if a tuple is a point (w = 1.0)
bool is_point(Tuple t) 
{
    return t.w == 1.0;
}

// Function to check if a tuple is a vector (w = 0.0)
bool is_vector(Tuple t) 
{
    return t.w == 0.0;
}

// Function to compare two tuples (for test validation)
bool tuple_equal(Tuple t1, Tuple t2) 
{
    return equal(t1.x, t2.x) && equal(t1.y, t2.y) && equal(t1.z, t2.z) && equal(t1.w, t2.w);
}

// Function to add two tuples
Tuple tuple_add(Tuple t1, Tuple t2)
{
    return tuple(t1.x + t2.x, t1.y + t2.y, t1.z + t2.z, t1.w + t2.w);
}

// Function to subtract two tuples
Tuple tuple_subtract(Tuple t1, Tuple t2) 
{
    return tuple(t1.x - t2.x, t1.y - t2.y, t1.z - t2.z, t1.w - t2.w);
}

// Function to subtract a vector from a point
Tuple subtract_vector_from_point(Tuple point, Tuple vector) 
{
    // Ensure the w component remains 1.0 for a point
    return tuple(point.x - vector.x, point.y - vector.y, point.z - vector.z, 1.0);
}

// Function to negate a vector (subtract it from the zero vector)
Tuple negate_vector(Tuple v)
{
    Tuple zero = vector(0, 0, 0);
    return tuple_subtract(zero, v);
}

// Function to negate a tuple
Tuple negate_tuple(Tuple t) 
{
    return tuple(-t.x, -t.y, -t.z, -t.w);
}

// Function to multiply a tuple by a scalar
Tuple tuple_multiply(Tuple t, double scalar) 
{
    return tuple(t.x * scalar, t.y * scalar, t.z * scalar, t.w * scalar);
}

// Function to divide a tuple by a scalar, same as multiplying with a fraction lets say 0.5
Tuple tuple_divide(Tuple t, double scalar) 
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
double magnitude(Tuple v) 
{
    return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

// Function to compare magnitude results
bool magnitude_equal(Tuple v, double expected_magnitude) 
{
    return equal(magnitude(v), expected_magnitude);
}

// Function to normalize a vector
Tuple normalize(Tuple v) 
{
    double mag = magnitude(v);
    if (mag == 0) {
        // Handling the case where magnitude is zero (though it's not expected for valid vectors)
        return vector(0, 0, 0);
    }
    return vector(v.x / mag, v.y / mag, v.z / mag);
}

// Function to compute the dot product of two vectors
double dot(Tuple a, Tuple b) 
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Function to compute the cross product of two vectors
Tuple cross(Tuple a, Tuple b)
{
    return vector(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Test scenarios
void test_scenarios() 
{
    // Scenario: A tuple with w=1.0 is a point
    Tuple a = tuple(4.3, -4.2, 3.1, 1.0);
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
    Tuple p = point(4, -4, 3);
    Tuple expected_point = tuple(4, -4, 3, 1.0);
    printf("p is correct: %s\n", tuple_equal(p, expected_point) ? "true" : "false"); // Expected output: true

    // Scenario: vector() creates tuples with w=0
    Tuple v = vector(4, -4, 3);
    Tuple expected_vector = tuple(4, -4, 3, 0.0);
    printf("v is correct: %s\n", tuple_equal(v, expected_vector) ? "true" : "false"); // Expected output: true

    // Scenario: Adding two tuples
    Tuple a1 = tuple(3, -2, 5, 1);
    Tuple a2 = tuple(-2, 3, 1, 0);
    Tuple result = tuple_add(a1, a2);
    Tuple expected = tuple(1, 1, 6, 1);
    printf("a1 + a2 = tuple(1, 1, 6, 1): %s\n", tuple_equal(result, expected) ? "true" : "false"); // Expected output: true

    // Scenario: Subtracting two points
    Tuple p1 = point(3, 2, 1);
    Tuple p2 = point(5, 6, 7);
    Tuple result_subtract = tuple_subtract(p1, p2);
    Tuple expected_subtract = vector(-2, -4, -6);
    printf("p1 - p2 = vector(-2, -4, -6): %s\n", tuple_equal(result_subtract, expected_subtract) ? "true" : "false"); // Expected output: true

    // Scenario: Subtracting a vector from a point
    Tuple p3 = point(3, 2, 1);
    Tuple v1 = vector(5, 6, 7);
    Tuple result_point_subtract_vector = subtract_vector_from_point(p3, v1);
    Tuple expected_point1 = point(-2, -4, -6);
    printf("p3 - v1 = point(-2, -4, -6): %s\n", tuple_equal(result_point_subtract_vector, expected_point1) ? "true" : "false"); // Expected output: true

    // Scenario: Subtracting two vectors
    Tuple v2 = vector(3, 2, 1);
    Tuple v3 = vector(5, 6, 7);
    Tuple result_subtract_vectors = tuple_subtract(v2, v3);
    Tuple expected_vector_subtract = vector(-2, -4, -6);
    printf("v2 - v3 = vector(-2, -4, -6): %s\n", tuple_equal(result_subtract_vectors, expected_vector_subtract) ? "true" : "false"); // Expected output: true

    // Scenario: Negating a vector (subtracting it from zero vector)
    Tuple zero = vector(0, 0, 0);
    Tuple v4 = vector(1, -2, 3);
    Tuple result_negate_vector = negate_vector(v4);
    Tuple expected_negated_vector = vector(-1, 2, -3);
    printf("zero - v4 = vector(-1, 2, -3): %s\n", tuple_equal(result_negate_vector, expected_negated_vector) ? "true" : "false"); // Expected output: true

    // Scenario: Negating a tuple
    Tuple p4 = tuple(1, -2, 3, -4);
    Tuple expected_negated_p4 = tuple(-1, 2, -3, 4);
    Tuple negated_p4 = negate_tuple(p4);
    printf("Negating a tuple (1, -2, 3, -4): %s\n", tuple_equal(negated_p4, expected_negated_p4) ? "true" : "false"); // Expected output: true

    // Scenario: Multiplying a tuple by a scalar
    Tuple a3 = tuple(1, -2, 3, -4);
    Tuple result_multiply_scalar = tuple_multiply(a3, 3.5);
    Tuple expected_multiply_scalar = tuple(3.5, -7, 10.5, -14);
    printf("a * 3.5 = tuple(3.5, -7, 10.5, -14): %s\n", tuple_equal(result_multiply_scalar, expected_multiply_scalar) ? "true" : "false"); // Expected output: true

    // Scenario: Multiplying a tuple by a fraction
    Tuple result_multiply_fraction = tuple_multiply(a3, 0.5);
    Tuple expected_multiply_fraction = tuple(0.5, -1, 1.5, -2);
    printf("a * 0.5 = tuple(0.5, -1, 1.5, -2): %s\n", tuple_equal(result_multiply_fraction, expected_multiply_fraction) ? "true" : "false"); // Expected output: true

    // Scenario: Dividing a tuple by a scalar
    Tuple result_divide_scalar = tuple_divide(a3, 2);
    Tuple expected_divide_scalar = tuple(0.5, -1, 1.5, -2);
    printf("a / 2 = tuple(0.5, -1, 1.5, -2): %s\n", tuple_equal(result_divide_scalar, expected_divide_scalar) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(1, 0, 0)
    Tuple v5 = vector(1, 0, 0);
    printf("Magnitude of vector(1, 0, 0) = 1: %s\n", magnitude_equal(v5, 1) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(0, 1, 0)
    Tuple v6 = vector(0, 1, 0);
    printf("Magnitude of vector(0, 1, 0) = 1: %s\n", magnitude_equal(v6, 1) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(0, 0, 1)
    Tuple v7 = vector(0, 0, 1);
    printf("Magnitude of vector(0, 0, 1) = 1: %s\n", magnitude_equal(v7, 1) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(1, 2, 3)
    Tuple v8 = vector(1, 2, 3);
    printf("Magnitude of vector(1, 2, 3) = √14: %s\n", magnitude_equal(v8, sqrt(14)) ? "true" : "false"); // Expected output: true

    // Scenario: Computing the magnitude of vector(-1, -2, -3)
    Tuple v9 = vector(-1, -2, -3);
    printf("Magnitude of vector(-1, -2, -3) = √14: %s\n", magnitude_equal(v9, sqrt(14)) ? "true" : "false"); // Expected output: true

    // Scenario: Normalizing vector(4, 0, 0)
    Tuple v10 = vector(4, 0, 0);
    Tuple result_normalize_v10 = normalize(v10);
    Tuple expected_normalize_v10 = vector(1, 0, 0);
    printf("Normalizing vector(4, 0, 0): %s\n", tuple_equal(result_normalize_v10, expected_normalize_v10) ? "true" : "false"); // Expected output: true

    // Scenario: Normalizing vector(1, 2, 3)
    Tuple v11 = vector(1, 2, 3);
    Tuple result_normalize_v11 = normalize(v11);
    // Vector(1/√14, 2/√14, 3/√14)
    Tuple expected_normalize_v11 = vector(0.26726, 0.53452, 0.80178);
    printf("Normalizing vector(1, 2, 3): %s\n", tuple_equal(result_normalize_v11, expected_normalize_v11) ? "true" : "false"); // Expected output: true

    // Scenario: The magnitude of a normalized vector
    Tuple norm = normalize(v11);
    printf("Magnitude of normalized vector(1, 2, 3): %s\n", equal(magnitude(norm), 1) ? "true" : "false"); // Expected output: true

    // Scenario: The dot product of two vectors
    Tuple v12 = vector(1, 2, 3);
    Tuple v13 = vector(2, 3, 4);
    double result_dot = dot(v12, v13);
    double expected_dot = 1*2 + 2*3 + 3*4; // 2 + 6 + 12 = 20
    printf("Dot product of vector(1, 2, 3) and vector(2, 3, 4): %s\n", equal(result_dot, expected_dot) ? "true" : "false"); // Expected output: true

    // Scenario: The cross product of two vectors
    Tuple v14 = vector(1, 2, 3);
    Tuple v15 = vector(2, 3, 4);
    
    Tuple result_cross_v14_v15 = cross(v14, v15);
    Tuple expected_cross_v14_v15 = vector(-1, 2, -1);
    printf("Cross product of vector(1, 2, 3) and vector(2, 3, 4): %s\n", tuple_equal(result_cross_v14_v15, expected_cross_v14_v15) ? "true" : "false"); // Expected output: true

    Tuple result_cross_v15_v14 = cross(v15, v14);
    Tuple expected_cross_v15_v14 = vector(1, -2, 1);
    printf("Cross product of vector(2, 3, 4) and vector(1, 2, 3): %s\n", tuple_equal(result_cross_v15_v14, expected_cross_v15_v14) ? "true" : "false"); // Expected output: true

    // Scenario: Adding colors
    Color c1 = {0.9, 0.6, 0.75};
    Color c2 = {0.7, 0.1, 0.25};
    Color result_add = add_colors(c1, c2);
    Color expected_add = {1.6, 0.7, 1.0};
    printf("Adding colors: ");
    print_color(result_add);
    printf("Test %s\n", compare_colors(result_add, expected_add) ? "Passed" : "Failed");

    // Scenario: Subtracting colors
    Color color_result_subtract = subtract_colors(c1, c2);
    Color color_expected_subtract = {0.2, 0.5, 0.5};
    printf("Subtracting colors: ");
    print_color(color_result_subtract);
    printf("Test %s\n", compare_colors(color_result_subtract, color_expected_subtract) ? "Passed" : "Failed");

    // Scenario: Multiplying a color by a scalar
    Color c3 = {0.2, 0.3, 0.4};
    Color color_result_multiply_scalar = multiply_color_scalar(c3, 2);
    Color color_expected_multiply_scalar = {0.4, 0.6, 0.8};
    printf("Multiplying color by scalar: ");
    print_color(color_result_multiply_scalar);
    printf("Test %s\n", compare_colors(color_result_multiply_scalar, color_expected_multiply_scalar) ? "Passed" : "Failed");

    // Scenario: Multiplying colors (Hadamard product)
    Color c4 = {1, 0.2, 0.4};
    Color c5 = {0.9, 1, 0.1};
    Color result_multiply_colors = multiply_colors(c4, c5);
    Color expected_multiply_colors = {0.9, 0.2, 0.04};
    printf("Multiplying colors: ");
    print_color(result_multiply_colors);
    printf("Test %s\n", compare_colors(result_multiply_colors, expected_multiply_colors) ? "Passed" : "Failed");

    Color c6 = {1.0, 0.2, 0.4};
    Color c7 = {0.9, 1.0, 0.1};

    Color blended_color = hadamard_product(c6, c7);

    printf("Blended Color: ");
    print_color(blended_color);

    // Test 2x2 Matrix
    t_matrix *m = create_2x2_matrix(-3, 5, 1, -2);
    (t_matrix_get(m, 0, 0) == -3);
    (t_matrix_get(m, 0, 1) == 5);
    (t_matrix_get(m, 1, 0) == 1);
    (t_matrix_get(m, 1, 1) == -2);

    // Test 3x3 Matrix
    t_matrix *m2 = create_3x3_matrix(-3, 5, 0, 1, -2, -7, 0, 1, 1);
    (t_matrix_get(m2, 0, 0) == -3);
    (t_matrix_get(m2, 1, 1) == -2);
    (t_matrix_get(m2, 2, 2) == 1);

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