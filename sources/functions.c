#include "../includes/minirt.h"

// using color = t_vec3;

void write_color(t_vec3 vec, t_var *var, int x, int y)
{
    // int color = ft_pixel(vector3->e[0],vector3->e[1],vector3->e[2],255);
    float r = vec.e[0];
    float g = vec.e[1];
    float b = vec.e[2];

    // Translate the [0,1] component values to the byte range [0,255].
    int ir = (int)(255.999 * r);
    int ig = (int)(255.999 * g);
    int ib = (int)(255.999 * b);
    // printf("[0] %i [1] %i [2]%i\n", ir, ig, ib);

    int color = ft_pixel(ir, ig, ib, 255);
    if (color < 0)
        color = color * 1;
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

t_sphere sphere_create(t_vec3 center, float radius)
{
    t_sphere s;

    hittable_init(&s.base, sphere_hit);
    s.center = center;
    s.radius = fmax(0, radius);
    return s;
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

bool hittable_list_hit(const hittable_list *list, const t_ray *r, float tmin, float tmax, t_hit *rec)
{
    t_hit temp_rec;
    bool hit_anything = false;
    float closest_so_far = tmax;

    for (int i = 0; i < list->size; i++)
    {
        if (list->objects[i]->hit(list->objects[i], r, tmin, closest_so_far, &temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}
