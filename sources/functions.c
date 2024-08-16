/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:18 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/16 11:35:09 by mrinkine         ###   ########.fr       */
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

bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec)
{
    const t_sphere *s = (const t_sphere *)self;
    t_vec3 oc = t_vec3_subtract_vectors(&r->orig, &s->center);
    float a = t_vec3_dot(&r->dir, &r->dir);
    float h = t_vec3_dot(&oc, &r->dir);
    float c = t_vec3_dot(&oc, &oc) - s->radius * s->radius;

    float discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    float sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (h - sqrtd) / a;
    if (root < tmin || tmax < root)
    {
        root = (h + sqrtd) / a;
        if (root < tmin || tmax < root)
            return false;
    }

    rec->t = root;
    rec->p = ray_at(r, rec->t);
    t_vec3 outward_normal = t_vec3_subtract_vectors(&rec->p, &s->center);
    outward_normal = t_vec3_multiply_scalar(&outward_normal, 1.0 / s->radius);
    set_face_normal(rec, r, &outward_normal);

    return true;
}
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
