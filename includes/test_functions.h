/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 13:56:05 by tvalimak          #+#    #+#             */
/*   Updated: 2024/09/01 16:47:35 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TEST_FUNCTIONS_H
# define TEST_FUNCTIONS_H

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

t_matrix*   submatrix(const t_matrix *m, int remove_row, int remove_col);
float       minor(t_matrix *m, int row, int col);
float       determinant(t_matrix *m);
float       determinant_2x2(const t_matrix *m);
float       determinant_3x3(const t_matrix *m);
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
Tuple       tuple(double x, double y, double z, double w);
Color       hadamard_product(Color c1, Color c2);
bool        compare_colors(Color c1, Color c2);
Color       add_colors(Color c1, Color c2);
Color       subtract_colors(Color c1, Color c2);
Color       multiply_color_scalar(Color c, double scalar);
Color       multiply_colors(Color c1, Color c2);
void        print_color(Color c);
Tuple       point(double x, double y, double z);
Tuple       vector(double x, double y, double z);
bool        is_point(Tuple t);
bool        is_vector(Tuple t);
bool        tuple_equal(Tuple t1, Tuple t2);
Tuple       tuple_add(Tuple t1, Tuple t2);
Tuple       tuple_subtract(Tuple t1, Tuple t2);
Tuple       subtract_vector_from_point(Tuple point, Tuple vector);
Tuple       negate_vector(Tuple v);
Tuple       negate_tuple(Tuple t);
Tuple       tuple_multiply(Tuple t, double scalar);
Tuple       tuple_divide(Tuple t, double scalar);
double      magnitude(Tuple v);
bool        magnitude_equal(Tuple v, double expected_magnitude);
Tuple       normalize(Tuple v);
double      dot(Tuple a, Tuple b);
Tuple       cross(Tuple a, Tuple b);
float       cofactor(t_matrix *m, int row, int col);
bool        is_invertible(t_matrix *m);

#endif