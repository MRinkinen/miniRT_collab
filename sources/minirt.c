/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/09/04 21:40:07 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"
//#include "../includes/test_functions.h"

// Function to multiply a tuple by a scalar
Tuple tuple_multiply(Tuple t, double scalar) 
{
    return tuple(t.x * scalar, t.y * scalar, t.z * scalar, t.w);  // Preserves w
}

// Computes a point along the ray at parameter t
Tuple position(Ray r, double t) 
{
    // p = origin + t * direction
    Tuple scaled_direction = tuple_multiply(r.direction, t);
    return tuple_add(r.origin, scaled_direction);
}

void print_clock_face(t_var *var)
{
    float CLOCK_RADIUS = var->image_width * 1.0 / 8.0;
    float CENTER_X = var->image_width / (float)2.0;
    float CENTER_Y = var->image_height / (float)2.0;
    const int MARK_SIZE = 10; // Size of the mark, 10 pixels

    Tuple twelve_oclock = point(0, 0, 1); // 12 o'clock at the top center
    int hours = 12;
    float angle_per_hour = (float)3.1415 / 6; // 2 * M_PI / 12

	printf("Center: (%f, %f)\n", CENTER_X, CENTER_Y);
	printf("Camera fov: %f\n", var->fov);

    // Draw each hour mark
    for (int hour = 0; hour < hours; hour++) {
        t_matrix *rotation = rotation_y(angle_per_hour * hour);
        Tuple hour_pos = apply_transformation(rotation, &twelve_oclock);

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

void printimage(void *param, t_map *map)
{
	t_var *var;
 	var = param;
	(void)map;
	for (int j = 0; j < (int)var->image_height; j++)
	{
		for (int i = 0; i < SCREEN_WIDTH; i++)
		{
			write_color(t_color_create(1,1,1), var, i, j);
		}
	}
	print_clock_face(var);
}

// Multiplies a transformation matrix by a tuple and returns the transformed tuple
Tuple apply_transformation(t_matrix *transformation, Tuple *point)
{
    t_matrix *point_matrix = tuple_to_matrix(point);
    t_matrix *transformed_matrix = t_matrix_multiply(transformation, point_matrix);
    Tuple transformed_point = matrix_to_tuple(transformed_matrix);
    free(point_matrix);
    free(transformed_matrix);
    return transformed_point;
}

// Function to convert the first column of a 4x4 matrix back to a Tuple
Tuple matrix_to_tuple(t_matrix *m)
{
    return tuple(m->data[0][0], m->data[1][0], m->data[2][0], m->data[3][0]);
}

// Function to convert a Tuple to a 4x4 matrix for transformation
t_matrix *tuple_to_matrix(Tuple *t)
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
bool tuple_equal(Tuple t1, Tuple t2) 
{
    return equal(t1.x, t2.x) && equal(t1.y, t2.y) && equal(t1.z, t2.z) && equal(t1.w, t2.w);
}
/*
// Function to check if two tuples (points or vectors) are equal
int tuple_equal(Tuple *a, Tuple *b) {
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
Tuple multiply_matrix_tuple(t_matrix *m, Tuple *p) {
    Tuple result;
    
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
Tuple tuple(double x, double y, double z, double w)
{
    Tuple t;
    t.x = x;
    t.y = y;
    t.z = z;
    t.w = w;
    return (t);
}
/*
Color hadamard_product(Color c1, Color c2) 
{
    Color result;
    result.red = c1.red * c2.red;
    result.green = c1.green * c2.green;
    result.blue = c1.blue * c2.blue;
    return result;
}*/

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
	printimage(&var, map);
	hooks(&var);
	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	terminate_data(map, "program ended successfully\n");
	return (EXIT_SUCCESS);
}
