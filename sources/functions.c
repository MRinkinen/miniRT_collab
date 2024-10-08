/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:18 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/08 16:15:39 by tvalimak         ###   ########.fr       */
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
    // Translate the [0,1] component values to the byte range [0,255].
    // int ir = (int)(255.999 * col.r);
    // int ig = (int)(255.999 * col.b);
    // int ib = (int)(255.999 * col.g);
    //printf("R %f G %f B %f \n", col.r, col.b, col.g);
    int color = ft_pixel(col.r, col.b, col.g, 255);
    mlx_put_pixel(var->testimage, x, y, color);
}

// void hittable_init(t_hittable *h, hit_func func)
// {
//     h->hit = func;
// }

// bool sphere_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec)
// {
//     const t_sphere *s = (const t_sphere *)self;
//     t_vec3 oc = t_vec3_subtract_vectors(&r->origin, &s->center);
//     float a = t_vec3_dot(&r->direction, &r->direction);
//     float h = t_vec3_dot(&oc, &r->direction);
//     float c = t_vec3_dot(&oc, &oc) - s->radius * s->radius;

//     float discriminant = h * h - a * c;

//     if (discriminant < 0)
//         return false;

//     float sqrtd = sqrtf(discriminant);
//     float root = (-h - sqrtd) / a;
//     if (root < tmin || root > tmax)
//     {
//         root = (-h + sqrtd) / a;
//         if (root < tmin || root > tmax)
//             return false;
//     }

//     rec->t = root;
//     rec->p = ray_at(r, rec->t);
//     t_vec3 outward_normal = t_vec3_subtract_vectors(&rec->p, &s->center);
//     outward_normal = t_vec3_multiply_scalar(&outward_normal, 1.0 / s->radius);
//     set_face_normal(rec, r, &outward_normal);

//     rec->color = s->color;
//     return true;
// }

// // experimental
// bool cylinder_hit(const t_hittable *self, const t_ray *r, float tmin, float tmax, t_hit *rec)
// {
//     const t_cylinders *c = (const t_cylinders *)self;

//     // Transform the ray origin to the cylinder's local space
//     t_vec3 oc = t_vec3_subtract_vectors(&r->origin, &c->center);

//     // Assume the cylinder is aligned along the y-axis
//     float a = r->direction.x * r->direction.x + r->direction.z * r->direction.z;
//     float b = 2.0 * (oc.x * r->direction.x + oc.z * r->direction.z);
//     float c_val = oc.x * oc.x + oc.z * oc.z - c->radius * c->radius;

//     // Solve the quadratic equation
//     float discriminant = b * b - 4 * a * c_val;
//     if (discriminant < 0)
//         return false;

//     float sqrtd = sqrtf(discriminant);

//     // Find the nearest root that lies within the acceptable range
//     float root = (-b - sqrtd) / (2.0 * a);
//     if (root < tmin || root > tmax)
//     {
//         root = (-b + sqrtd) / (2.0 * a);
//         if (root < tmin || root > tmax)
//             return false;
//     }

//     // Calculate the intersection point
//     float y_hit = r->origin.y + root * r->direction.y;

//     // Check if the intersection point is within the bounds of the cylinder's height
//     if (y_hit < c->center.y || y_hit > c->center.y + c->height)
//         return false;

//     rec->t = root;
//     rec->p = ray_at(r, rec->t);

//     // Calculate the normal at the intersection point
//     t_vec3 outward_normal = t_vec3_create(rec->p.x - c->center.x, 0, rec->p.z - c->center.z);
//     outward_normal = t_vec3_multiply_scalar(&outward_normal, 1.0 / c->radius);

//     set_face_normal(rec, r, &outward_normal);

//     rec->color = c->color;

//     return true;
// }

// t_sphere sphere_create(t_vec3 center, float radius, t_color col)
// {
//     t_sphere sphere;

//     hittable_init(&sphere.base, sphere_hit);
//     sphere.color = col;
//     sphere.center = center;
//     sphere.radius = fmax(0, radius);
//     return (sphere);
// }

// void set_face_normal(t_hit *rec, const t_ray *r, const t_vec3 *outward_normal)
// {
//     // Compute whether the ray hits the front face
//     rec->front_face = t_vec3_dot(&r->direction, outward_normal) < 0;

//     // Set the normal to point against the ray direction if front face, otherwise with it
//     if (rec->front_face)
//     {
//         rec->normal = *outward_normal;
//     }
//     else
//     {
//         t_vec3 neg_outward_normal = t_vec3_multiply_scalar(outward_normal, -1.0);
//         rec->normal = neg_outward_normal;
//     }
// }

// void hittable_list_init(hittable_list *list)
// {
//     list->size = 0;
// }

// void hittable_list_clear(hittable_list *list)
// {
//     list->size = 0;
// }

// void hittable_list_add(hittable_list *list, t_hittable *object)
// {
//     if (list->size < MAX_OBJECTS)
//     {
//         list->objects[list->size++] = object;
//     }
// }

// bool hittable_list_hit(const hittable_list *list, const t_ray *ray, float tmin, float tmax, t_hit *rec)
// {
//     t_hit temp_rec;
//     bool hit_anything = false;
//     float closest_so_far = tmax;

//     // Loop through all objects in the list
//     for (int i = 0; i < list->size; i++)
//     {
//         // Check if the current object is hit by the ray
//         if (list->objects[i]->hit(list->objects[i], ray, tmin, closest_so_far, &temp_rec))
//         {
//             hit_anything = true;                 // Mark that we hit something
//             closest_so_far = temp_rec.t;         // Update the closest hit point
//             *rec = temp_rec;                     // Update the hit record with the closest hit
//         }
//     }

//     return hit_anything; // Return true if any object was hit, false otherwise
// }

void initialize_camera(t_var *var, t_cam *camera, t_map *map)
{
    camera->position = point(map->camera->x,map->camera->y,map->camera->z);
    camera->fov = map->camera->fov;
    //camera->aspect_ratio = (float)SCREEN_WIDTH / (float)var->image_height;

    camera->viewport_height = 2.0;  // Arbitrary, could be based on FOV
    camera->viewport_width = camera->viewport_height * var->cam.aspect_ratio;
    camera->focal_length = 1.0;

    camera->horizontal = vector(var->cam.viewport_width, 0, 0);
    camera->vertical = vector(0, var->cam.viewport_height, 0);
    camera->lower_left_corner = tuple_subtract(tuple_subtract(tuple_subtract(camera->position, tuple_divide(camera->horizontal, 2)),
                                                              tuple_divide(camera->vertical, 2)),
                                               vector(0, 0, var->cam.focal_length));

    // t_tuple horizontal = vector(var->cam.viewport_width, 0, 0);
    // t_tuple vertical = vector(0, var->cam.viewport_height, 0);
    // t_tuple lower_left_corner = tuple_subtract(tuple_subtract(tuple_subtract(camera_position, tuple_divide(horizontal, 2)),
    //                                                           tuple_divide(vertical, 2)),
    //                                            vector(0, 0, var->cam.focal_length));
    // Calculate forward vector (direction the camera is looking at)
    //camera->forward = normalize_vector(t_vec3_subtract_vectors(&look_at, &position));

    // Ensure the up vector is normalized (important for the cross product)
    //up = normalize_vector(up);

    // Calculate right vector (horizontal direction, perpendicular to forward and up)
    //camera->right = normalize_vector(t_vec3_cross(&camera->forward, &up));

    // Recalculate up vector to ensure orthogonality (vertical direction, perpendicular to forward and right)
    //camera->up = normalize_vector(t_vec3_cross(&camera->right, &camera->forward));

}
