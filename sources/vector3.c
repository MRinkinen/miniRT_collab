#include "../includes/minirt.h"

#ifndef t_vec3_H
#define t_vec3_H

t_vec3 t_vec3_create(double e0, double e1, double e2) {
    t_vec3 result = { {e0, e1, e2} };
    return result;
}

double t_vec3_x(const t_vec3* v) { return v->e[0]; }
double t_vec3_y(const t_vec3* v) { return v->e[1]; }
double t_vec3_z(const t_vec3* v) { return v->e[2]; }

t_vec3 t_vec3_negate(const t_vec3* v) {
    return t_vec3_create(-v->e[0], -v->e[1], -v->e[2]);
}

double t_vec3_get(const t_vec3* v, int i) { return v->e[i]; }
void t_vec3_set(t_vec3* v, int i, double value) { v->e[i] = value; }

t_vec3* t_vec3_add(t_vec3* v, const t_vec3* u) {
    v->e[0] += u->e[0];
    v->e[1] += u->e[1];
    v->e[2] += u->e[2];
    return v;
}

t_vec3* t_vec3_multiply(t_vec3* v, double t) {
    v->e[0] *= t;
    v->e[1] *= t;
    v->e[2] *= t;
    return v;
}

t_vec3* t_vec3_divide(t_vec3* v, double t) {
    return t_vec3_multiply(v, 1/t);
}

double t_vec3_length(const t_vec3* v) {
    return sqrt(v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2]);
}

double t_vec3_length_squared(const t_vec3* v) {
    return v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2];
}

void t_vec3_print(const t_vec3* v) {
    printf("%f %f %f\n", v->e[0], v->e[1], v->e[2]);
}

t_vec3 t_vec3_add_vectors(const t_vec3* u, const t_vec3* v) 
{
    return t_vec3_create(u->e[0] + v->e[0], u->e[1] + v->e[1], u->e[2] + v->e[2]);
}

t_vec3 t_vec3_subtract_vectors(const t_vec3* u, const t_vec3* v) {
    return t_vec3_create(u->e[0] - v->e[0], u->e[1] - v->e[1], u->e[2] - v->e[2]);
}

t_vec3 t_vec3_multiply_vectors(const t_vec3* u, const t_vec3* v) {
    return t_vec3_create(u->e[0] * v->e[0], u->e[1] * v->e[1], u->e[2] * v->e[2]);
}

t_vec3 t_vec3_multiply_scalar(const t_vec3* v, double t) {
    return t_vec3_create(t * v->e[0], t * v->e[1], t * v->e[2]);
}

t_vec3 t_vec3_divide_scalar(const t_vec3* v, double t) {
    return t_vec3_multiply_scalar(v, 1/t);
}

double t_vec3_dot(const t_vec3* u, const t_vec3* v) {
    return u->e[0] * v->e[0] + u->e[1] * v->e[1] + u->e[2] * v->e[2];
}

t_vec3 t_vec3_cross(const t_vec3* u, const t_vec3* v) {
    return t_vec3_create(u->e[1] * v->e[2] - u->e[2] * v->e[1],
                       u->e[2] * v->e[0] - u->e[0] * v->e[2],
                       u->e[0] * v->e[1] - u->e[1] * v->e[0]);
}

t_vec3 t_vec3_unit_vector(const t_vec3* v) {
    return t_vec3_divide_scalar(v, t_vec3_length(v));
}

#endif
// t_t_vec3 t_t_vec3_create(double e0, double e1, double e2) 
// {
//     t_t_vec3 result = { {e0, e1, e2} };
//     return result;
// }

// double t_t_vec3_x(const t_t_vec3* v) 
// { 
//         return v->e[0]; 
// }

// double t_t_vec3_y(const t_t_vec3* v) 
// { 
//         return v->e[1]; 
// }

// double t_t_vec3_z(const t_t_vec3* v) 
// { 
//     return v->e[2]; 
// }

// t_t_vec3 t_t_vec3_negate(const t_t_vec3* v) 
// {
//     return t_t_vec3_create(-v->e[0], -v->e[1], -v->e[2]);
// }

// double t_t_vec3_get(const t_t_vec3* v, int i) 
// { 
//     return v->e[i]; 
// }
// void t_t_vec3_set(t_t_vec3* v, int i, double value) 
// { 
//     v->e[i] = value;
// }

// t_t_vec3* t_t_vec3_add(t_t_vec3* v, const t_t_vec3* u) 
// {
//     v->e[0] += u->e[0];
//     v->e[1] += u->e[1];
//     v->e[2] += u->e[2];
//     return v;
// }

// t_t_vec3* t_t_vec3_multiply(t_t_vec3* v, double t) 
// {
//     v->e[0] *= t;
//     v->e[1] *= t;
//     v->e[2] *= t;
//     return v;
// }

// t_t_vec3* t_t_vec3_divide(t_t_vec3* v, double t) 
// {
//     return t_t_vec3_multiply(v, 1/t);
// }

// double t_t_vec3_length(const t_t_vec3* v) 
// {
//     return sqrt(v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2]);
// }

// double t_t_vec3_length_squared(const t_t_vec3* v) 
// {
//     return v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2];
// }

// void t_t_vec3_print(const t_t_vec3* v) 
// {
//     // DELETE THIS (maeby?)
//     printf("%f %f %f\n", v->e[0], v->e[1], v->e[2]);
// }

// t_t_vec3 t_t_vec3_add_vectors(const t_t_vec3* u, const t_t_vec3* v) 
// {
//     return t_t_vec3_create(u->e[0] + v->e[0], u->e[1] + v->e[1], u->e[2] + v->e[2]);
// }

// t_t_vec3 t_t_vec3_subtract_vectors(const t_t_vec3* u, const t_t_vec3* v) 
// {
//     return t_t_vec3_create(u->e[0] - v->e[0], u->e[1] - v->e[1], u->e[2] - v->e[2]);
// }

// t_t_vec3 t_t_vec3_multiply_vectors(const t_t_vec3* u, const t_t_vec3* v) 
// {
//     return t_t_vec3_create(u->e[0] * v->e[0], u->e[1] * v->e[1], u->e[2] * v->e[2]);
// }

// t_t_vec3 t_t_vec3_multiply_scalar(const t_t_vec3* v, double t) 
// {
//     return t_t_vec3_create(t * v->e[0], t * v->e[1], t * v->e[2]);
// }

// t_t_vec3 t_t_vec3_divide_scalar(const t_t_vec3* v, double t) 
// {
//     return t_t_vec3_multiply_scalar(v, 1/t);
// }

// double t_t_vec3_dot(const t_t_vec3* u, const t_t_vec3* v) 
// {
//     return u->e[0] * v->e[0] + u->e[1] * v->e[1] + u->e[2] * v->e[2];
// }

// t_t_vec3 t_t_vec3_cross(const t_t_vec3* u, const t_t_vec3* v) 
// {
//     return t_t_vec3_create(u->e[1] * v->e[2] - u->e[2] * v->e[1],
//                        u->e[2] * v->e[0] - u->e[0] * v->e[2],
//                        u->e[0] * v->e[1] - u->e[1] * v->e[0]);
// }

// t_t_vec3 t_t_vec3_unit_vector(const t_t_vec3* v) 
// {
//     return t_t_vec3_divide_scalar(v, t_t_vec3_length(v));
// }