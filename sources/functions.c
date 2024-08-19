/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:18 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/19 12:56:29 by mrinkine         ###   ########.fr       */
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
    // int color = ft_pixel(vector3->e[0],vector3->e[1],vector3->e[2],255);
    // float r = col.r;
    // float g = vec.y;
    // float b = vec.z;
    // if (col.r < 0)
    //     col.r = col.r * 1;
    // if (col.b < 0)
    //     col.b = col.b * 1;
    // if (col.g < 0)
    //     col.g = col.g * 1;

    // if (col.r > 255)
    //     col.r = 255;
    // if (col.b > 255)
    //     col.b = 255;
    // if (col.g > 255)
    //     col.g = 255;

    // Translate the [0,1] component values to the byte range [0,255].
    int ir = (int)(255.999 * col.r);
    int ig = (int)(255.999 * col.b);
    int ib = (int)(255.999 * col.g);
    // printf("[0] %i [1] %i [2]%i\n", ir, ig, ib);

    int color = ft_pixel(ir, ig, ib, 255);
    // int color = ft_pixel((int)col.r, (int)col.b, (int)col.g, 255);

    // if (color < 0)
    //     color = color * 1;

    // printf("Color = %i", color);
    mlx_put_pixel(var->testimage, x, y, color);
}

void hittable_init(t_hittable *h, hit_func func)
{
    h->hit = func;
}

// bool ray_intersects_sphere(const t_hittable *self, const t_ray *ray, float tmin, float tmax, t_hit *rec)
// {
//     const t_sphere *sphere = (const t_sphere *)self;
//     t_vec3 oc = t_vec3_subtract_vectors(&ray.orig, &sphere.center);

//     float a = t_vec3_dot(&ray.dir, &ray.dir);
//     float b = 2.0f * t_vec3_dot(&oc, &ray.dir);
//     float c = t_vec3_dot(&oc, &oc) - sphere.radius * sphere.radius;

//     float discriminant = b * b - 4 * a * c;

//     if (discriminant < 0)
//     {
//         return false; // No intersection
//     }
//     else
//     {
//         float sqrt_discriminant = sqrtf(discriminant);

//         // Find the nearest point of intersection
//         float t0 = (-b - sqrt_discriminant) / (2.0f * a);
//         float t1 = (-b + sqrt_discriminant) / (2.0f * a);

//         if (t0 > t1)
//             t0 = t1;

//         // If t0 is negative, t1 could be the solution
//         if (t0 < 0.0f)
//         {
//             t0 = t1;
//             if (t0 < 0.0f)
//                 return false; // Both t0 and t1 are negative
//         }

//         *t = t0;
//         return true; // Intersection found
//     }
// }

// bool sphere_hit(const t_hittable *self, const t_ray *ray, t_hit *rec, t_vec3 *intersection_point)
// {
//     const t_sphere *s = (const t_sphere *)self;

//     t_vec3 oc = t_vec3_subtract_vectors(&ray->orig, &s->center);

//     float a = t_vec3_dot(&ray->dir, &ray->dir);
//     float b = t_vec3_dot(&oc, &ray->dir);
//     float c = t_vec3_dot(&oc, &oc) - s->radius * s->radius;

//     float discriminant = b * b - a * c;
//     if (discriminant < 0)
//         return false;
//     else
//     {
//         float sqrt_discriminant = sqrtf(discriminant);

//         // Find the nearest point of intersection
//         float t0 = (-b - sqrt_discriminant) / (2.0f * a);
//         float t1 = (-b + sqrt_discriminant) / (2.0f * a);

//         if (t0 > t1)
//             t0 = t1;

//         // If t0 is negative, t1 could be the solution
//         if (t0 < 0.0f)
//         {
//             t0 = t1;
//             if (t0 < 0.0f)
//                 return false; // Both t0 and t1 are negative
//         }

//         // Calculate the intersection point using the smallest positive t
//         *intersection_point = calculate_intersection_point(*ray, t0);
//         return true; // Intersection found
//     }

//     // float sqrtd = sqrt(discriminant);

//     // // Find the nearest root that lies in the acceptable range.
//     // float root = (b - sqrtd) / a;
//     // if (root < tmin || tmax < root)
//     // {
//     //     root = (b + sqrtd) / a;
//     //     if (root < tmin || tmax < root)
//     //         return false;
//     // }
//     // rec->t = root;
//     // rec->p = ray_at(r, rec->t);
//     // t_vec3 outward_normal = t_vec3_subtract_vectors(&rec->p, &s->center);
//     // outward_normal = t_vec3_multiply_scalar(&outward_normal, 1.0 / s->radius);
//     // set_face_normal(rec, r, &outward_normal);

//     // return true;
// }

bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec)
{
    const t_sphere *s = (const t_sphere *)self;
    t_vec3 oc = t_vec3_subtract_vectors(&r->orig, &s->center);
    float a = t_vec3_dot(&r->dir, &r->dir);
    float h = t_vec3_dot(&oc, &r->dir);
    float c = t_vec3_dot(&oc, &oc) - s->radius * s->radius;

    float discriminant = h * h - a * c;
    // if (discriminant > 0)
    //     printf("Discriminant: %f\n", discriminant);

    if (discriminant < 0)
        return false;

    float sqrtd = sqrtf(discriminant);
    float root = (-h - sqrtd) / a;
    if (root < tmin || root > tmax)
    {
        root = (-h + sqrtd) / a;
        if (root < tmin || root > tmax)
            return false;
    }

    rec->t = root;
    rec->p = ray_at(r, rec->t);
    t_vec3 outward_normal = t_vec3_subtract_vectors(&rec->p, &s->center);
    outward_normal = t_vec3_multiply_scalar(&outward_normal, 1.0 / s->radius);
    set_face_normal(rec, r, &outward_normal);

    rec->color = s->color;
    // printf("Hit at t = %f, Point = (%f, %f, %f)\n", rec->t, rec->p.x, rec->p.y, rec->p.z);

    return true;
}

// bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec)
// {
//     const t_sphere *s = (const t_sphere *)self;

//     // oc is the vector from the ray's origin to the center of the sphere
//     t_vec3 oc = t_vec3_subtract_vectors(&r->orig, &s->center);

//     float a = t_vec3_dot(&r->dir, &r->dir);                 // Dot product of direction with itself (should be 1 if dir is normalized)
//     float h = t_vec3_dot(&oc, &r->dir);                     // Dot product of oc and direction
//     float c = t_vec3_dot(&oc, &oc) - s->radius * s->radius; // Length squared minus radius squared

//     float discriminant = h * h - a * c;
//     if (discriminant < 0)
//         return false; // No intersection, ray misses the sphere

//     float sqrtd = sqrtf(discriminant);

//     // Find the nearest root that lies within the acceptable range
//     float root = (-h - sqrtd) / a; // The closer intersection
//     if (root < tmin || root > tmax)
//     {
//         root = (-h + sqrtd) / a; // The farther intersection
//         if (root < tmin || root > tmax)
//             return false; // Both intersections are out of bounds
//     }

//     rec->t = root;
//     rec->p = ray_at(r, rec->t); // Calculate the hit point using the ray equation

//     // Calculate the outward normal and normalize it
//     t_vec3 outward_normal = t_vec3_subtract_vectors(&rec->p, &s->center);
//     outward_normal = t_vec3_multiply_scalar(&outward_normal, 1.0 / s->radius);

//     // Set the normal and determine if the ray is hitting the front face or inside
//     set_face_normal(rec, r, &outward_normal);

//     return true;
// }

// bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec)
// {
//     const t_sphere *s = (const t_sphere *)self;
//     t_vec3 oc = t_vec3_subtract_vectors(&r->orig, &s->center);
//     float a = t_vec3_dot(&r->dir, &r->dir);
//     float h = t_vec3_dot(&oc, &r->dir);
//     float c = t_vec3_dot(&oc, &oc) - s->radius * s->radius;

//     float discriminant = h * h - a * c;
//     if (discriminant < 0)
//         return false;

//     float sqrtd = sqrt(discriminant);

//     // Find the nearest root that lies in the acceptable range.
//     float root = (h - sqrtd) / a;
//     if (root < tmin || tmax < root)
//     {
//         root = (h + sqrtd) / a;
//         if (root < tmin || tmax < root)
//             return false;
//     }

//     rec->t = root;
//     rec->p = ray_at(r, rec->t);
//     t_vec3 outward_normal = t_vec3_subtract_vectors(&rec->p, &s->center);
//     outward_normal = t_vec3_multiply_scalar(&outward_normal, 1.0 / s->radius);
//     set_face_normal(rec, r, &outward_normal);

//     return true;
// }

t_sphere sphere_create(t_vec3 center, float radius, t_color col)
{
    t_sphere sphere;

    hittable_init(&sphere.base, sphere_hit);
    sphere.color = col;
    sphere.center = center;
    sphere.radius = fmax(0, radius);
    return (sphere);
}

void set_face_normal(t_hit *rec, const t_ray *r, const t_vec3 *outward_normal)
{
    // Compute whether the ray hits the front face
    rec->front_face = t_vec3_dot(&r->dir, outward_normal) < 0;

    // Set the normal to point against the ray direction if front face, otherwise with it
    if (rec->front_face)
    {
        rec->normal = *outward_normal;
    }
    else
    {
        t_vec3 neg_outward_normal = t_vec3_multiply_scalar(outward_normal, -1.0);
        rec->normal = neg_outward_normal;
    }
}

void hittable_list_init(hittable_list *list)
{
    list->size = 0;
}

void hittable_list_clear(hittable_list *list)
{
    list->size = 0;
}

void hittable_list_add(hittable_list *list, t_hittable *object)
{
    if (list->size < MAX_OBJECTS)
    {
        list->objects[list->size++] = object;
    }
}

bool hittable_list_hit(const hittable_list *list, const t_ray *ray, float tmin, float tmax, t_hit *rec)
{
    t_hit temp_rec;
    bool hit_anything = false;
    float closest_so_far = tmax;

    for (int i = 0; i < list->size; i++)
    {
        if (list->objects[i]->hit(list->objects[i], ray, tmin, closest_so_far, &temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}
