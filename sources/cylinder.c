/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:18:40 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/22 13:16:10 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

void create_cylinders(t_var *var, t_map *map, int *obj_index)
{
	t_cylinders *current_cylinder;
	t_tuple orientation;
	t_tuple center;
	float radius;
	float height;
	t_color color;

	current_cylinder = map->cylinders;
	while (current_cylinder != NULL)
	{
		orientation = normalize(vector(current_cylinder->nx, current_cylinder->ny, current_cylinder->nz));
        center = point(current_cylinder->x, current_cylinder->y, current_cylinder->z);
        radius = current_cylinder->diameter / 2.0f;
        height = current_cylinder->height;
        color = t_color_create(current_cylinder->r, current_cylinder->g, current_cylinder->b);
        var->objects[*obj_index].data.cylinder.translation_matrix = translation(center.x, center.y, center.z);
        var->objects[*obj_index].data.cylinder.rotation_matrix = rotation_from_normal(orientation);
        var->objects[*obj_index].data.cylinder.scaling_matrix = scaling(radius, 1.0f, radius); // Scale by radius in xz and height in y
        var->objects[*obj_index].data.cylinder.transform = t_matrix_multiply(t_matrix_multiply(var->objects[*obj_index].data.cylinder.scaling_matrix, var->objects[*obj_index].data.cylinder.rotation_matrix), var->objects[*obj_index].data.cylinder.translation_matrix);
        var->objects[*obj_index].data.cylinder.inverse_transform = inverse(var->objects[*obj_index].data.cylinder.transform);
        var->objects[*obj_index].type = CYLINDER;
        var->objects[*obj_index].data.cylinder.center = center;
        var->objects[*obj_index].data.cylinder.radius = radius;
        var->objects[*obj_index].data.cylinder.height = height;
        var->objects[*obj_index].data.cylinder.color = color;
        var->objects[*obj_index].data.cylinder.orientation = orientation;
        current_cylinder = current_cylinder->next;
		(*obj_index)++;
	}
}

// Function to calculate the normal vector at a point on the cylinder's surface
t_tuple calculate_cylinder_normal(const t_cylinder *cylinder, const t_tuple *point)
{
    t_tuple local_point = apply_transformation(cylinder->inverse_transform, point);
    t_tuple center_to_point;
    t_tuple normal;
    float half_height = cylinder->height / 2.0;

    // Check if the point is on the top cap
    if (local_point.y >= half_height - 1e-6)
    {
        normal = tuple(0.0f, 1.0f, 0.0f, 0);
    }
    // Check if the point is on the bottom cap
    else if (local_point.y <= -half_height + 1e-6)
    {
        normal = tuple(0.0f, -1.0f, 0.0f, 0);
    }
    // Otherwise, the point is on the cylindrical surface
    else
    {
        center_to_point = tuple_subtract(local_point, tuple(0.0f, local_point.y, 0.0f, 1));
        normal = tuple(center_to_point.x, 0.0f, center_to_point.z, 0);
        normal = normalize(normal);
    }

    // Transform the normal back to world coordinates
    normal = apply_transformation(transpose(inverse(cylinder->inverse_transform)), &normal);
    return normalize(normal);
}

t_tuple	tuple_scale(t_tuple t, double scalar)
{
	return (tuple_multiply(t, scalar));
}

bool intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, float *t)
{
    t_tuple oc = tuple_subtract(ray->origin, cylinder->center);
    t_tuple direction = ray->direction;
    t_tuple axis = cylinder->orientation;
    t_ray transformed_ray;
    float a, b, c, discriminant, t0, t1, temp, half_height, dist0, dist1, t_cap, cap_dist;
    bool hit = false;

    // Transform the ray into the cylinder's local coordinate system
    transformed_ray.origin = apply_transformation(cylinder->inverse_transform, &ray->origin);
    transformed_ray.direction = apply_transformation(cylinder->inverse_transform, &ray->direction);

    // Check intersection with the cylindrical surface
    a = dot(direction, direction) - pow(dot(direction, axis), 2);
    b = 2.0 * (dot(direction, oc) - dot(direction, axis) * dot(oc, axis));
    c = dot(oc, oc) - pow(dot(oc, axis), 2) - cylinder->radius * cylinder->radius;
    discriminant = b * b - 4 * a * c;

    if (discriminant >= 0)
	{
        t0 = (-b - sqrt(discriminant)) / (2.0 * a);
        t1 = (-b + sqrt(discriminant)) / (2.0 * a);

        if (t0 > t1) {
            temp = t0;
            t0 = t1;
            t1 = temp;
        }

        half_height = cylinder->height / 2.0;
        t_tuple point0 = tuple_add(ray->origin, tuple_multiply(ray->direction, t0));
        t_tuple point1 = tuple_add(ray->origin, tuple_multiply(ray->direction, t1));
        dist0 = dot(tuple_subtract(point0, cylinder->center), axis);
        dist1 = dot(tuple_subtract(point1, cylinder->center), axis);

        if (dist0 >= -half_height && dist0 <= half_height) {
            *t = t0;
            hit = true;
        } else if (dist1 >= -half_height && dist1 <= half_height) {
            *t = t1;
            hit = true;
        }
    }

    // Check intersection with the top cap
    t_tuple cap_center = tuple_add(cylinder->center, tuple_multiply(axis, half_height));
    t_cap = (dot(cap_center, axis) - dot(ray->origin, axis)) / dot(ray->direction, axis);
    t_tuple cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
    cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), tuple_subtract(cap_point, cap_center)));

    if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t)) {
        *t = t_cap;
        hit = true;
    }

    // Check intersection with the bottom cap
    cap_center = tuple_subtract(cylinder->center, tuple_multiply(axis, half_height));
    t_cap = (dot(cap_center, axis) - dot(ray->origin, axis)) / dot(ray->direction, axis);
    cap_point = tuple_add(ray->origin, tuple_multiply(ray->direction, t_cap));
    cap_dist = sqrt(dot(tuple_subtract(cap_point, cap_center), tuple_subtract(cap_point, cap_center)));

    if (cap_dist <= cylinder->radius && t_cap >= 0 && (!hit || t_cap < *t)) {
        *t = t_cap;
        hit = true;
    }

    return hit;
}
