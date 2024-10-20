/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/21 00:22:31 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"


bool intersect_object(const t_ray *ray, const t_object *object, float *t)
{
    if (object->type == SPHERE)
    {
        return (intersect_sphere(ray, &object->data.sphere, t));
    }
    else if (object->type == CYLINDER)
    {
        return (intersect_cylinder(ray, &object->data.cylinder, t));
    }
    else if (object->type == PLANE)
    {
        return (intersect_plane(ray, &object->data.plane, t));
    }
    else
    {
        return (false);
    }
}

bool find_closest_intersection(t_var *var, const t_ray *ray, t_object **closest_object, float *closest_t)
{
    bool hit;
    float t;
    int i;

    i = 0;
    hit = false;
    *closest_t = FLT_MAX;
    while (i < var->num_objects)
    {
        if (intersect_object(ray, &var->objects[i], &t) && t < *closest_t && t > 0.001f)
        {
            *closest_t = t;
            *closest_object = &var->objects[i];
            hit = true;
        }
        i++;
    }
    return (hit);
}

t_tuple calculate_normal(const t_object *object, const t_tuple *point)
{
    if (object->type == SPHERE)
    {
        return (calculate_sphere_normal(&object->data.sphere, point));
    }
    else if (object->type == CYLINDER)
    {
        return (calculate_cylinder_normal(&object->data.cylinder, point));
    }
    else if (object->type == PLANE)
    {
        return (object->data.plane.normal);
    }
    else
    {
        return (vector(0, 0, 0));
    }
}

void printimage(void *param)
{
    t_var *var;
    t_ray r;
    t_color pixel_color;
    t_tuple intersection_point;
    t_tuple normal;
    t_tuple view_dir;
    t_color object_color;
    t_object *closest_object;
    bool hit;
    float closest_t;
    int y;
    int x;

    y = 0;
    x = 0;
    var = param;
    while (y < (int)var->image_height)
    {
        x = 0;
        while (x < SCREEN_WIDTH)
        {
            r = generate_ray_for_pixel(var, x, y);
            pixel_color = var->ambientl;
            hit = false;
            closest_object = NULL;
            if (find_closest_intersection(var, &r, &closest_object, &closest_t))
            {
                intersection_point = tuple_add(r.origin, tuple_multiply(r.direction, closest_t));
                normal = calculate_normal(closest_object, &intersection_point);
                if (closest_object->type == SPHERE) {
                    object_color = closest_object->data.sphere.color;
                }
                else if (closest_object->type == CYLINDER)
                {
                    object_color = closest_object->data.cylinder.color;
                }
                else if (closest_object->type == PLANE)
                {
                    object_color = closest_object->data.plane.color;
                }
                hit = true;
            }
            // Calculate lighting if an intersection was found
            if (hit) {
                t_tuple view_dir = normalize(tuple_subtract(var->cam.position, intersection_point));
                pixel_color = calculate_phong_lighting(&intersection_point, &normal, &var->test_light[0], &object_color, &view_dir, var->objects, var->num_objects);
            }
            // Write the pixel color to the image
            write_color(pixel_color, var, x, y);
            x++;
        }
        y++;
    }
}
// fix the orientations
// check that is there fish-eyeing
// norm-proof stuff

int main(int argc, char **argv)
{
	t_var var;
	t_element_count element_count;
	t_map *map;

	if (argc != 2)
	{
		ft_printf("Error in arguments, just give a map filename\n");
		return (0);
	}
	ft_memset(&element_count, 0, sizeof(t_element_count));
	map = malloc(sizeof(t_map));
	setup_data(&element_count, map);
	if (!map)
		return (0);
	if (read_to_parse(&element_count, map, argv) == 0)
		return (0);
	print_data(map);
	printf("image width: %f\n", var.image_width);
	printf("image height: %f\n", var.image_height);
	if (mlxinit(&var) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    init_ambient_color(&var, map);
    initialize_camera(&var, &var.cam, map);
    initialize_scene(&var, map);
    init_light(&var, map); // TESTI LIGHT!!!!
	printimage(&var);
	hooks(&var);
	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	terminate_data(map, &var, "program ended successfully\n");
    free_scene(&var);
	return (EXIT_SUCCESS);
}
