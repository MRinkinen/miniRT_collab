/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/08/29 12:42:14 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

void printimage(void *param, t_map *map)
{
	t_var *var;
 	var = param;
	(void)map;
	for (int j = 0; j < (int)var->image_height; j++)
	{
		for (int i = 0; i < SCREEN_WIDHT; i++)
		{
			write_color(t_color_create(1,0,0), var, i, j); // Red screeen comes from here :D
		}
	}
}

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
	if (mlxinit(&var, map) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	printimage(&var, map);
	hooks(&var);
	mlx_loop(var.mlx);
	mlx_terminate(var.mlx);
	terminate_data(map, "program ended successfully\n");
	return (EXIT_SUCCESS);
}

// t_color calculate_final_color(t_color object_color, t_color ambient_light, t_color light_color, t_vec3 light_dir, t_vec3 normal, t_vec3 view_dir, float shininess)
// {
// 	t_color ambient = color_multiply_scalar(ambient_light, 0.1f); // Ambient component

// 	// Diffuse component
// 	float diff = fmax(t_vec3_dot(&normal, &light_dir), 0.0f);
// 	t_color diffuse = color_multiply_scalar(light_color, diff);

// 	// Specular component
// 	t_vec3 reflect_dir = reflect_vector(light_dir, normal);
// 	float spec = pow(fmax(t_vec3_dot(&view_dir, &reflect_dir), 0.0f), shininess);
// 	t_color specular = color_multiply_scalar(light_color, spec);

// 	// Combine all components
// 	t_color final_color = color_add(ambient, color_add(diffuse, specular));
// 	final_color = color_add(final_color, object_color);

// 	// Clamp and normalize the final color
// 	final_color = clamp_and_normalize_color(final_color);

// 	return final_color;
// }

// t_color ray_color(t_var *var, const t_ray *r, hittable_list *world)
// {
// 	t_hit rec;
// 	t_color background = {0.5f, 0.7f, 1.0f}; // Example background color

// 	if (hittable_list_hit(world, r, 0.001f, INFINITY, &rec))
// 	{
// 		//printf("HIT %f %f %f\n", rec.p.x, rec.p.y, rec.p.z);
// 		// Calculate the distance from the camera to the intersection point
// 		float distance = calculate_distance(var->camera_center, rec.p);

// 		// Define attenuation factors (tuned for your scene)
// 		float constant = 1.0;
// 		float linear = 0.09;
// 		float quadratic = 0.032;

// 		// Calculate the attenuation based on the distance
// 		float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));
// 		t_color object_color = color_multiply_scalar(rec.color, attenuation);

// 		// Define light properties
// 		t_color light_color = {1.0f, 1.0f, 1.0f}; // Example light color
// 		t_vec3 light_dir = normalize_vector(t_vec3_subtract_vectors(&var->light_position, &rec.p));
// 		t_vec3 view_dir = normalize_vector(t_vec3_subtract_vectors(&var->camera_center, &rec.p));
// 		float shininess = 32.0f; // Example shininess value

// 		t_color final_color = calculate_final_color(object_color, var->ambientl.color, light_color, light_dir, rec.normal, view_dir, shininess);
// 		//t_color final_color = t_color_create(1.0f, 0.0f, 0.0f);
// 		return final_color;
// 	}
// 	//printf("NO HIT\n");
// 	// Return background color if no hit occurs
// 	return background;
// }

// void intitscreen(t_var *var, t_map *map)
// {
// 	(void)map;
// 	// var->aspect_ratio = (float)SCREEN_WIDHT / var->image_height;

// 	// float theta = var->camera_fov * (PI / 180.0f);

// 	float focal_length = 1.0;

// 	// float viewport_height = 1.0; // Jos ei 1, niin pallukat soikeita
// 	// float viewport_width = viewport_height * ((float)SCREEN_WIDHT / var->image_height);
// 	// float viewport_height = 2.0f * tanf(theta / 2.0f);
// 	float viewport_width = var->viewport_height * var->cam.aspect_ratio;
// 	var->camera_center = var->cam.position;

// 	t_vec3 viewport_u = t_vec3_create(viewport_width, 0.0f, 0.0f);
// 	t_vec3 viewport_v = t_vec3_create(0.0f, -var->viewport_height, 0.0f);

// 	var->pixel_delta_u = t_vec3_divide_scalar(&viewport_u, SCREEN_WIDHT);
// 	var->pixel_delta_v = t_vec3_divide_scalar(&viewport_v, var->image_height);

// 	t_vec3 temp1 = t_vec3_create(0.0f, 0.0f, -focal_length);
// 	t_vec3 temp2 = t_vec3_divide_scalar(&viewport_u, 2.0f);
// 	t_vec3 temp3 = t_vec3_divide_scalar(&viewport_v, 2.0f);
// 	t_vec3 temp4 = t_vec3_add_vectors(&temp1, &temp2);
// 	t_vec3 temp5 = t_vec3_add_vectors(&temp4, &temp3);
// 	t_vec3 viewport_upper_left = t_vec3_subtract_vectors(&var->camera_center, &temp5);

// 	t_vec3 temp = t_vec3_add_vectors(&var->pixel_delta_u, &var->pixel_delta_v);
// 	t_vec3 temp_result = t_vec3_multiply_scalar(&temp, 0.5f);
// 	var->pixel00_loc = t_vec3_add_vectors(&viewport_upper_left, &temp_result);
// }

// void screenloop(t_var *var, hittable_list world)
// {
// 	t_vec3 temp_u;
// 	t_vec3 temp_v;
// 	t_vec3 pixel_center;
// 	t_vec3 ray_direction;
// 	t_ray ray;
// 	t_color pixel_color;

// 	for (int j = 0; j < (int)var->image_height; j++)
// 	{
// 		for (int i = 0; i < SCREEN_WIDHT; i++)
// 		{
// 			// printf("VITTUU %i\n", i);
// 			temp_u = t_vec3_multiply_scalar(&var->pixel_delta_u, i);
// 			temp_v = t_vec3_multiply_scalar(&var->pixel_delta_v, j);
// 			pixel_center = t_vec3_add_vectors(&var->pixel00_loc, &temp_u);
// 			pixel_center = t_vec3_add_vectors(&pixel_center, &temp_v);
// 			ray_direction = t_vec3_subtract_vectors(&pixel_center, &var->camera_center);
// 			ray = ray_create(&var->camera_center, &ray_direction);
// 			pixel_color = ray_color(var, &ray, &world);
// 			write_color(pixel_color, var, i, j);
// 		}
// 	}
// }
// /*Experimental*/
// t_cylinders addcylinder(t_vec3 vec, t_color color, float diameter, float height)
// {
// 	t_cylinders cylinder;

// 	hittable_init(&cylinder.base, cylinder_hit); // Assume a function similar to sphere_hit for cylinders
// 	cylinder.center = vec;
// 	cylinder.color = color;
// 	cylinder.radius = fmax(0, diameter);
// 	cylinder.height = fmax(0, height);

// 	return (cylinder);
// }

// t_sphere addsphere(t_vec3 vec, t_color col, float radius)
// {
// 	t_sphere sphere;

// 	// sphere = sphere_create(vec, 100, col);
// 	hittable_init(&sphere.base, sphere_hit);
// 	sphere.color = col;
// 	sphere.center = vec;
// 	sphere.radius = fmax(0, radius);

// 	return (sphere);
// }

// void printimage(void *param, t_map *map)
// {
// 	t_var *var;
// 	var = param;

// 	hittable_list world;

// 	hittable_list_init(&world);

// 	int i = 0;
// 	int sphere_count = map->element_count->sphere;
// 	int cylinder_count = map->element_count->cylinder;

// 	t_sphere *spheres = malloc(sizeof(t_sphere) * sphere_count);
// 	t_cylinders *cylinders = malloc(sizeof(t_cylinder) * cylinder_count);
// 	var->ambientl.color = t_color_create(map->ambient->r, map->ambient->g, map->ambient->b);
// 	// var->ambientl.intensity = map->ambient->ratio;
// 	var->ambientl.color = color_multiply_scalar(var->ambientl.color, map->ambient->ratio);
// 	var->ambientl.color = clamp_and_normalize_color(var->ambientl.color);

// 	/*TEST LIGHT*/
// 	var->light_position = t_vec3_create(0.0f, 0.0f, 0.0f);
// 	printf("%f %f %f\n", var->ambientl.color.r, var->ambientl.color.b, var->ambientl.color.g);
// 	while (map->spheres)
// 	{
// 		spheres[i] = addsphere(t_vec3_create(map->spheres->x, map->spheres->y, map->spheres->z), t_color_create(map->spheres->r, map->spheres->g, map->spheres->b), map->spheres->diameter);
// 		hittable_list_add(&world, (t_hittable *)&spheres[i]);
// 		map->spheres = map->spheres->next;
// 		i++;
// 	}
// 	i = 0;
// 	while (map->cylinder)
// 	{
// 		cylinders[i] = addcylinder(t_vec3_create(map->cylinder->x, map->cylinder->y, map->cylinder->z), t_color_create(map->cylinder->r, map->cylinder->g, map->cylinder->b), map->cylinder->diameter, map->cylinder->height);
// 		hittable_list_add(&world, (t_hittable *)&cylinders[i]);
// 		map->cylinder = map->cylinder->next;
// 		i++;
// 	}

// 	intitscreen(var, map);
// 	screenloop(var, world);
// }


