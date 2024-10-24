/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:18 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/24 12:03:53 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void initialize_camera(t_var *var, t_cam *camera, t_map *map)
{
	var->cam.position = point(map->camera->x, map->camera->y, map->camera->z);
	var->aspect_ratio = (float) WIDTH / HEIGHT;
	var->cam.focal_length = (WIDTH / 2) / (tanf((map->camera->fov * (PI / 180)) / 2));
	var->cam.view_w = WIDTH * 2;
	var->cam.viewp_h = var->cam.view_w / var->aspect_ratio;
	var->cam.forward = normalize(vector(map->camera->nx, map->camera->ny, map->camera->nz));
	if (magnitude(cross(vector(0, 1, 0), var->cam.forward)) != 0)
		var->cam.right = cross(vector(0, 1, 0), var->cam.forward);
	else
		var->cam.right = cross(vector(0, 1, -0.00001), var->cam.forward);
	var->cam.up = cross(var->cam.forward, var->cam.right);
	var->cam.view_u = tuple_multiply(var->cam.right, var->cam.view_w);
	var->cam.view_v = tuple_multiply(var->cam.up, var->cam.viewp_h);
	var->cam.delta_u = tuple_divide(var->cam.view_u, WIDTH);
	var->cam.delta_v = tuple_divide(var->cam.view_v, HEIGHT);
	var->cam.focal = tuple_multiply(var->cam.forward, var->cam.focal_length);
	var->cam.v_up_left_c = tuple_subtract(point(map->camera->x, map->camera->y, map->camera->z), var->cam.focal);
	var->cam.v_up_left_c = tuple_subtract(var->cam.v_up_left_c, tuple_divide(var->cam.view_u, 2));
	var->cam.v_up_left_c = tuple_subtract(var->cam.v_up_left_c, tuple_divide(var->cam.view_v, 2));
	var->cam.loc_00 = tuple_multiply(tuple_add(var->cam.delta_u, var->cam.delta_v), 0.5);
	var->cam.loc_00 = tuple_add(var->cam.loc_00, var->cam.v_up_left_c);
	// add printf's to all values with te w component
	printf("camera position: %f %f %f %f\n", var->cam.position.x, var->cam.position.y, var->cam.position.z, var->cam.position.w);
	printf("camera forward: %f %f %f %f\n", var->cam.forward.x, var->cam.forward.y, var->cam.forward.z, var->cam.forward.w);
	printf("camera right: %f %f %f %f\n", var->cam.right.x, var->cam.right.y, var->cam.right.z, var->cam.right.w);
	printf("camera up: %f %f %f %f\n", var->cam.up.x, var->cam.up.y, var->cam.up.z, var->cam.up.w);
	printf("camera view_u: %f %f %f %f\n", var->cam.view_u.x, var->cam.view_u.y, var->cam.view_u.z, var->cam.view_u.w);
	printf("camera view_v: %f %f %f %f\n", var->cam.view_v.x, var->cam.view_v.y, var->cam.view_v.z, var->cam.view_v.w);
	printf("camera delta_u: %f %f %f %f\n", var->cam.delta_u.x, var->cam.delta_u.y, var->cam.delta_u.z, var->cam.delta_u.w);
	printf("camera delta_v: %f %f %f %f\n", var->cam.delta_v.x, var->cam.delta_v.y, var->cam.delta_v.z, var->cam.delta_v.w);
	printf("camera focal: %f %f %f %f\n", var->cam.focal.x, var->cam.focal.y, var->cam.focal.z, var->cam.focal.w);
	printf("camera v_up_left_c: %f %f %f %f\n", var->cam.v_up_left_c.x, var->cam.v_up_left_c.y, var->cam.v_up_left_c.z, var->cam.v_up_left_c.w);
	printf("camera loc_00: %f %f %f %f\n", var->cam.loc_00.x, var->cam.loc_00.y, var->cam.loc_00.z, var->cam.loc_00.w);
}

/*
void initialize_camera(t_var *var, t_cam *camera, t_map *map)
{
    float half_height;
    float half_width;
    t_tuple lookfrom;
    t_tuple lookat;
    t_tuple vup; // Assume the up vector is always (0, 1, 0)

    half_height = tan(map->camera->fov * (PI / 180.0f) / 2);
    half_width = var->cam.aspect_ratio  * half_height;
    lookfrom = point(map->camera->x, map->camera->y, map->camera->z);
    lookat = point(map->camera->nx, map->camera->ny, map->camera->nz);
    printf("lookat: %f %f %f\n", lookat.x, lookat.y, lookat.z);
    vup = vector(0.0f, -1.0f, 0.0f);
    var->cam.position = point(map->camera->x, map->camera->y, map->camera->z);
    camera->w = normalize(tuple_subtract(lookfrom, lookat));
    camera->u = normalize(cross(vup, camera->w));
    camera->v = cross(camera->w, camera->u);
    camera->lower_left_corner = tuple_subtract(
        tuple_subtract(
            tuple_subtract(var->cam.position, tuple_multiply(camera->u, half_width)),
            tuple_multiply(camera->v, half_height)),
        camera->w);
    camera->horizontal = tuple_multiply(camera->u, 2 * half_width);
    camera->vertical = tuple_multiply(camera->v, 2 * half_height);
}*/

void	fill_rotation_values(float *values, t_tuple axis, \
t_rotation_params params)
{
    float	x;
    float	y;
    float	z;

    x = axis.x;
    y = axis.y;
    z = axis.z;

    values[0] = params.cos_theta + x * x * params.one_minus_cos;
    values[1] = x * y * params.one_minus_cos - z * params.sin_theta;
    values[2] = x * z * params.one_minus_cos + y * params.sin_theta;
    values[3] = 0;
    values[4] = y * x * params.one_minus_cos + z * params.sin_theta;
    values[5] = params.cos_theta + y * y * params.one_minus_cos;
    values[6] = y * z * params.one_minus_cos - x * params.sin_theta;
    values[7] = 0;
    values[8] = z * x * params.one_minus_cos - y * params.sin_theta;
    values[9] = z * y * params.one_minus_cos + x * params.sin_theta;
    values[10] = params.cos_theta + z * z * params.one_minus_cos;
    values[11] = 0;
    values[12] = 0;
    values[13] = 0;
    values[14] = 0;
    values[15] = 1;
}

t_matrix	*rotation_around_axis(t_tuple axis, float angle)
{
    t_matrix			*rotation;
    t_rotation_params	params;
    float				values[16];

    params.cos_theta = cos(angle);
    params.sin_theta = sin(angle);
    params.one_minus_cos = 1.0f - params.cos_theta;

    fill_rotation_values(values, axis, params);

    rotation = create_4x4_matrix(values);
    return (rotation);
}

t_tuple	get_default_normal(void)
{
    t_tuple	default_normal;

    default_normal = vector(0, 1, 0);
    return (normalize(default_normal));
}

bool	check_special_case(t_tuple v2)
{
    if (fabs(v2.x - 1.0) < EPSILON && fabs(v2.y) < EPSILON && fabs(v2.z) < EPSILON)
        return (true);
    if (fabs(v2.z - 1.0) < EPSILON && fabs(v2.x) < EPSILON && fabs(v2.y) < EPSILON)
        return (true);
    return (false);
}

t_matrix	*handle_special_case(t_tuple v2)
{
    if (fabs(v2.x - 1.0) < EPSILON)
    {
        // For left-hand system, the rotation angle should be +PI/2 to rotate in the opposite direction
        return (rotation_around_axis(vector(0, 0, 1), PI / 2));
    }
    return (identity_matrix());
}

t_tuple get_arbitrary_axis(t_tuple v1)
{
    t_tuple	arbitrary_axis;

    arbitrary_axis = vector(1, 0, 0);
    if (fabs(v1.x) > fabs(v1.y))
        arbitrary_axis = vector(0, 1, 0);
    return (arbitrary_axis);
}

t_matrix	*handle_axis_length_case(t_tuple v1, t_tuple v2)
{
    t_tuple	axis;
    t_tuple	arbitrary_axis;

    if (dot(v1, v2) > 0)
        return (identity_matrix());
    arbitrary_axis = get_arbitrary_axis(v1);
    axis = normalize(cross(v2, arbitrary_axis));  // Swapped operands for left-hand system
    return (rotation_around_axis(axis, PI));  // The rotation remains the same as PI is the same in both systems
}

t_matrix	*rotation_from_normal(t_tuple normal)
{
    t_tuple	v1;
    t_tuple	v2;
    t_tuple	axis;
    float	axis_length;
    float	angle;

    v1 = get_default_normal();
    v2 = normalize(normal);
    if (check_special_case(v2))
    {
        return (handle_special_case(v2));
    }
    axis = cross(v2, v1);  // Inverted for left-hand system
    axis_length = magnitude(axis);
    if (axis_length < EPSILON)
    {
        return (handle_axis_length_case(v1, v2));
    }
    axis = normalize(axis);
    angle = acos(dot(v1, v2));
    return (rotation_around_axis(axis, angle));
}

// Function to multiply a tuple by a scalar
t_tuple tuple_multiply(t_tuple t, double scalar)
{
    return (tuple(t.x * scalar, t.y * scalar, t.z * scalar, t.w));
}

// Computes a point along the ray at parameter t
t_tuple position(t_ray r, double t)
{
    t_tuple	scaled_direction;

    scaled_direction = tuple_multiply(r.direction, t);
    return (tuple_add(r.origin, scaled_direction));
}

// Multiplies a transformation matrix by a tuple and returns the transformed tuple
t_tuple apply_transformation(t_matrix *transformation, t_tuple *point)
{
    t_matrix	*point_matrix;
    t_matrix	*transformed_matrix;
    t_tuple		transformed_point;

    point_matrix = tuple_to_matrix(point);
    transformed_matrix = t_matrix_multiply(transformation, point_matrix);
    transformed_point = matrix_to_tuple(transformed_matrix);
    free(point_matrix);
    free(transformed_matrix);
    return (transformed_point);
}

// Function to convert the first column of a 4x4 matrix back to a t_tuple
t_tuple matrix_to_tuple(t_matrix *m)
{
    return (tuple(m->data[0][0], m->data[1][0], m->data[2][0], m->data[3][0]));
}

t_matrix *tuple_to_matrix(t_tuple *t)
{
    float		values[16];
    t_matrix	*m;

    values[0] = t->x;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
    values[4] = t->y;
    values[5] = 0;
    values[6] = 0;
    values[7] = 0;
    values[8] = t->z;
    values[9] = 0;
    values[10] = 0;
    values[11] = 0;
    values[12] = t->w;
    values[13] = 0;
    values[14] = 0;
    values[15] = 0;
    m = create_4x4_matrix(values);
    return (m);
}

// Function to create a rotation matrix around the Z-axis (left-hand system)
t_matrix *rotation_z(float radians)
{
    t_matrix* rotation = identity_matrix(); // Initialize as identity matrix

    // Adjusted rotation for left-hand system
    rotation->data[0][0] = cos(radians);
    rotation->data[0][1] = -sin(radians); // Inverted for left-handed system
    rotation->data[1][0] = sin(radians);  // Inverted for left-handed system
    rotation->data[1][1] = cos(radians);

    return rotation;
}

// Function to create a rotation matrix around the Y-axis (left-hand system)
t_matrix* rotation_y(float radians)
{
    t_matrix *transform = identity_matrix(); // Initialize as identity matrix

    // Adjusted rotation for left-hand system
    transform->data[0][0] = cos(radians);
    transform->data[0][2] = sin(radians);    // Inverted for left-handed system
    transform->data[2][0] = -sin(radians);   // Inverted for left-handed system
    transform->data[2][2] = cos(radians);

    return transform;
}

// Function to create a rotation matrix for rotating around the X axis (left-hand system)
t_matrix *rotation_x(float radians)
{
    t_matrix *transform = identity_matrix();

    // Adjusted rotation for left-hand system
    transform->data[1][1] = cos(radians);
    transform->data[1][2] = -sin(radians);   // Inverted for left-handed system
    transform->data[2][1] = sin(radians);    // Inverted for left-handed system
    transform->data[2][2] = cos(radians);

    return transform;
}

// Function to create a reflection matrix (scaling by a negative value)
t_matrix *reflective_scaling(float x, float y, float z)
{
    // Use the scaling function to scale by negative values for reflection
    // Ensure negative values for the left-handed system reflection
    return scaling(x, y, z);
}

// Function to compute the inverse of a scaling matrix
t_matrix *inverse_scaling(float x, float y, float z)
{
    // Inverse scaling factors are the reciprocals of the original scaling factors
    // This remains the same for both left and right-hand systems
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
    // Set scaling components, these remain the same for left-hand systems
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
    // This function doesn't need changes for left-hand system
    return equal(t1.x, t2.x) && equal(t1.y, t2.y) && equal(t1.z, t2.z) && equal(t1.w, t2.w);
}

// Function to create the inverse of a translation matrix using while loops
t_matrix *inverse_translation(t_matrix *transform)
{
    // In the left-hand system, translation inversion remains the same
    return translation(-transform->data[0][3], -transform->data[1][3], -transform->data[2][3]);
}

// Function to multiply a 4x4 matrix by a point (assumed to be a 4x1 vector)
t_tuple multiply_matrix_tuple(t_matrix *m, t_tuple *p)
{
    // This function remains unchanged for left-hand systems
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
    // Initialize the identity matrix, this remains the same for left-hand systems
    t_matrix *transform = identity_matrix();

    // Set the translation components, no need to modify for left-hand system
    transform->data[0][3] = x;
    transform->data[1][3] = y;
    transform->data[2][3] = z;

    return (transform);
}

// Function to calculate the inverse of a matrix (remains the same for left-hand system)
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

// Function to create the cofactor matrix (no change for left-hand system)
t_matrix *cofactor_matrix(const t_matrix *m)
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

// Function to check if matrix is invertible (remains the same)
bool is_invertible(t_matrix *m)
{
    float det = determinant(m);
    return det != 0;
}

// Function to compute the cofactor of a matrix element
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

// Function to return the minor/determinant of a given matrix
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

// Function to create a 4x4 matrix from an array of values
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
    return (vector(v.x / mag, v.y / mag, v.z / mag));
}

// Function to compute the dot product of two vectors
float dot(t_tuple a, t_tuple b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Function to compute the cross product of two vectors in a left-handed system
t_tuple cross(t_tuple a, t_tuple b)
{
    return vector(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z, // Swap terms for left-handed system
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
            if (!equal(m1->data[i][j], m2->data[j][i])) { // Adjusted for left-handed system
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
float degrees_to_radians(float degrees)
{
    return degrees * (PI / 180.0f);
}

