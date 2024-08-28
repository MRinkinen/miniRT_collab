/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:15:10 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/19 12:05:49 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3 t_vec3_create(float x, float y, float z)
{
    t_vec3 result;

    result.x = x;
    result.y = y;
    result.z = z;
    return (result);
}

float calculate_distance(t_vec3 vec_a, t_vec3 vec_b)
{
    return sqrtf((vec_b.x - vec_a.x) * (vec_b.x - vec_a.x) +
                 (vec_b.y - vec_a.y) * (vec_b.y - vec_a.y) +
                 (vec_b.z - vec_a.z) * (vec_b.z - vec_a.z));
}

float t_vec3_x(const t_vec3 *vec)
{
    return (vec->x);
}

float t_vec3_y(const t_vec3 *vec)
{
    return (vec->y);
}

float t_vec3_z(const t_vec3 *vec)
{
    return (vec->z);
}

t_vec3 t_vec3_negate(const t_vec3 *vec)
{
    return (t_vec3_create(-vec->x, -vec->y, -vec->z));
}

float t_vec3_get(const t_vec3 *vec, char i)
{
    if (i == 'x')
        return (vec->x);
    else if (i == 'y')
        return (vec->y);
    else
        return (vec->z);
}

void t_vec3_set(t_vec3 *vec, char i, float value)
{
    if (i == 'x')
        vec->x = value;
    if (i == 'y')
        vec->y = value;
    if (i == 'z')
        vec->z = value;
}

t_vec3 *t_vec3_add(t_vec3 *vec, const t_vec3 *u)
{
    vec->x += u->x;
    vec->y += u->y;
    vec->z += u->z;
    return (vec);
}

t_vec3 *t_vec3_multiply(t_vec3 *vec, float t)
{
    vec->x *= t;
    vec->y *= t;
    vec->z *= t;
    return (vec);
}

t_vec3 *t_vec3_divide(t_vec3 *vec, float t)
{
    return (t_vec3_multiply(vec, 1 / t));
}
/*
Returns the length of this vector
*/
float t_vec3_magnitude(const t_vec3 *vec)
{
    return (sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z));
}
/*
Returns the squared length of this vector
*/
float t_vec3_magnitude_squared(const t_vec3 *vec)
{
    return (vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

void t_vec3_print(const t_vec3 *vec)
{
    printf("%f %f %f\n", vec->x, vec->y, vec->x);
}

t_vec3 t_vec3_add_vectors(const t_vec3 *u, const t_vec3 *v)
{
    return (t_vec3_create(u->x + v->x, u->y + v->y, u->z + v->z));
}

t_vec3 t_vec3_subtract_vectors(const t_vec3 *u, const t_vec3 *v)
{
    return (t_vec3_create(u->x - v->x, u->y - v->y, u->z - v->z));
}

t_vec3 t_vec3_multiply_vectors(const t_vec3 *u, const t_vec3 *v)
{
    return (t_vec3_create(u->x * v->x, u->y * v->y, u->z * v->z));
}

t_vec3 t_vec3_multiply_scalar(const t_vec3 *v, float t)
{
    return (t_vec3_create(t * v->x, t * v->y, t * v->z));
}

t_vec3 t_vec3_divide_scalar(const t_vec3 *v, float t)
{
    return (t_vec3_multiply_scalar(v, 1 / t));
}

float t_vec3_dot(const t_vec3 *vec_a, const t_vec3 *vec_b)
{
    return (vec_a->x * vec_b->x + vec_a->y * vec_b->y + vec_a->z * vec_b->z);
}

t_vec3 t_vec3_cross(const t_vec3 *u, const t_vec3 *v)
{
    return (t_vec3_create(u->y * v->z - u->z * v->y,
                          u->z * v->x - u->x * v->z,
                          u->x * v->y - u->y * v->x));
}

t_vec3 t_vec3_unit_vector(const t_vec3 *vec)
{
    return (t_vec3_divide_scalar(vec, t_vec3_magnitude(vec)));
}

t_vec3 calculate_intersection_point(t_ray ray, float t)
{
    t_vec3 intersection_point = {
        ray.orig.x + t * ray.orig.x,
        ray.orig.y + t * ray.orig.y,
        ray.orig.z + t * ray.orig.z};
    return intersection_point;
}

t_vec3 reflect_vector(t_vec3 v, t_vec3 n)
{
    float dot = t_vec3_dot(&v, &n);
    t_vec3 result;
    result.x = v.x - 2.0f * dot * n.x;
    result.y = v.y - 2.0f * dot * n.y;
    result.z = v.z - 2.0f * dot * n.z;
    return result;
}

t_vec3 normalize_vector(t_vec3 v)
{
    float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    t_vec3 result;
    result.x = v.x / magnitude;
    result.y = v.y / magnitude;
    result.z = v.z / magnitude;
    return result;
}
