#include "../includes/minirt.h"
#include "../includes/parsing.h"

void create_spheres(t_var *var, t_map *map, int *obj_index)
{
    t_spheres *current_sphere;
    t_tuple center;
    t_color color;
    float radius;

    current_sphere = map->spheres;
    while (current_sphere != NULL)
    {
        center = point(current_sphere->x, current_sphere->y, current_sphere->z);
        radius = current_sphere->diameter / 2.0f;
        color = t_color_create(current_sphere->r, current_sphere->g, current_sphere->b);
        var->objects[*obj_index].type = SPHERE;
        var->objects[*obj_index].data.sphere.center = center;
        var->objects[*obj_index].data.sphere.radius = radius;
        var->objects[*obj_index].data.sphere.color = color;
        current_sphere = current_sphere->next;
        (*obj_index)++;
    }
}

t_tuple calculate_sphere_normal(const t_sphere *sphere, const t_tuple *point)
{
    return normalize(tuple_subtract(*point, sphere->center));
}

bool intersect_sphere(const t_ray *ray, const t_sphere *sphere, float *t)
{
    t_tuple oc = tuple_subtract(ray->origin, sphere->center);
    float a = dot(ray->direction, ray->direction);
    float b = 2.0f * dot(oc, ray->direction);
    float c = dot(oc, oc) - sphere->radius * sphere->radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    } else {
        *t = (-b - sqrt(discriminant)) / (2.0f * a);
        return true;
    }
}

