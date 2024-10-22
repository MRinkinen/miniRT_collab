#include "../includes/minirt.h"
#include "../includes/parsing.h"


void create_planes(t_var *var, t_map *map, int *obj_index)
{
    t_planes *current_plane;
    t_tuple orientation;
    t_tuple center;
    t_tuple normal;
    t_color color;

    current_plane = map->planes;
    while (current_plane != NULL)
    {
        orientation = normalize(vector(current_plane->nx, current_plane->ny, current_plane->nz));
        center = point(current_plane->x, current_plane->y, current_plane->z);
        normal = vector(current_plane->nx, current_plane->ny, current_plane->nz);
        color = t_color_create(current_plane->r, current_plane->g, current_plane->b);
        var->objects[*obj_index].type = PLANE;
        var->objects[*obj_index].data.plane.center;
        var->objects[*obj_index].data.plane.point = center;
        var->objects[*obj_index].data.plane.normal = normal;
        var->objects[*obj_index].data.plane.color = color;
        current_plane = current_plane->next;
        (*obj_index)++;
    }
}


bool intersect_plane(const t_ray *ray, const t_plane *plane, float *t)
{
    float denom = dot(plane->normal, ray->direction);
    if (fabs(denom) > EPSILON)
    {
        t_tuple p0l0 = tuple_subtract(plane->point, ray->origin);
        *t = dot(p0l0, plane->normal) / denom;
        return (*t >= 0);
    }
    return false;
}
