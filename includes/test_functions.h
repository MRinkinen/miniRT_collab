/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 13:56:05 by tvalimak          #+#    #+#             */
/*   Updated: 2024/09/05 19:11:11 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TEST_FUNCTIONS_H
# define TEST_FUNCTIONS_H

#include "./minirt.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

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
}         t_tuple;

typedef struct
{
    t_tuple origin;
    t_tuple direction;
} t_ray;

typedef struct
{
    double red;
    double green;
    double blue;
} t_color;

t_ray         ray(t_tuple origin, t_tuple direction);
t_tuple       position(t_ray r, double t);
t_tuple       apply_transformation(t_matrix *transformation, t_tuple *point);
t_tuple       matrix_to_tuple(t_matrix *m);
t_matrix    *tuple_to_matrix(t_tuple *t);
t_matrix    *shearing(float xy, float xz, float yx, float yz, float zx, float zy);
t_matrix    *rotation_z(float radians);
t_matrix    *rotation_x(float radians);
t_matrix    *rotation_y(float radians);
t_matrix    *scaling(float x, float y, float z);
t_matrix    *inverse_scaling(float x, float y, float z);
t_matrix    *reflective_scaling(float x, float y, float z);
t_matrix    *submatrix(const t_matrix *m, int remove_row, int remove_col);
float       minor(const t_matrix *m, int row, int col);
float       determinant(const t_matrix *m);
float       determinant_2x2(const t_matrix *m);
float       determinant_3x3(const t_matrix *m);
t_matrix    *inverse(t_matrix *m);
t_matrix    *inverse_translation(t_matrix *transform);
t_matrix    *translation(float x, float y, float z);
t_matrix*   cofactor_matrix(const t_matrix *m);
t_matrix*   identity_matrix();
t_matrix*   transpose(t_matrix *m);
t_matrix*   t_matrix_multiply(t_matrix *a, t_matrix *b);
t_matrix    *create_2x2_matrix(float a, float b, float c, float d);
t_matrix    *create_3x3_matrix(float a, float b, float c,
                           float d, float e, float f,
                           float g, float h, float i);
t_matrix    *create_4x4_matrix(float a, float b, float c, float d,
                           float e, float f, float g, float h,
                           float i, float j, float k, float l,
                           float m, float n, float o, float p);
float       t_matrix_get(t_matrix *m, int row, int col);
bool        equal(double a, double b);
t_tuple       tuple(double x, double y, double z, double w);
t_color       hadamard_product(t_color c1, t_color c2);
bool        compare_colors(t_color c1, t_color c2);
t_color       add_colors(t_color c1, t_color c2);
t_color       subtract_colors(t_color c1, t_color c2);
t_color       multiply_color_scalar(t_color c, double scalar);
t_color       multiply_colors(t_color c1, t_color c2);
void        print_color(t_color c);
t_tuple       point(double x, double y, double z);
t_tuple       vector(double x, double y, double z);
bool        is_point(t_tuple t);
bool        is_vector(t_tuple t);
bool        tuple_equal(t_tuple t1, t_tuple t2);
t_tuple       tuple_add(t_tuple t1, t_tuple t2);
t_tuple       tuple_subtract(t_tuple t1, t_tuple t2);
t_tuple       subtract_vector_from_point(t_tuple point, t_tuple vector);
t_tuple       negate_vector(t_tuple v);
t_tuple       negate_tuple(t_tuple t);
t_tuple       tuple_multiply(t_tuple t, double scalar);
t_tuple       tuple_divide(t_tuple t, double scalar);
double      magnitude(t_tuple v);
bool        magnitude_equal(t_tuple v, double expected_magnitude);
t_tuple       normalize(t_tuple v);
double      dot(t_tuple a, t_tuple b);
t_tuple       cross(t_tuple a, t_tuple b);
float       cofactor(const t_matrix *m, int row, int col);
bool        is_invertible(t_matrix *m);

#endif
