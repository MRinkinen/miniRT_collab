/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs_n_lists.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 11:10:02 by tvalimak          #+#    #+#             */
/*   Updated: 2024/09/16 15:26:51 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

void free_matrix(t_matrix *matrix)
{
    if (matrix)
        free(matrix);
}

void free_sphere(t_sphere *sphere)
{
    if (sphere->transform)
        free_matrix(sphere->transform);

    if (sphere->inverse_transform)
        free_matrix(sphere->inverse_transform);

    if (sphere->translation_matrix)
        free_matrix(sphere->translation_matrix);

    if (sphere->rotation_matrix)
        free_matrix(sphere->rotation_matrix);

    if (sphere->scaling_matrix)
        free_matrix(sphere->scaling_matrix);
}

void free_plane(t_plane *plane)
{
    if (plane->transform)
        free_matrix(plane->transform);

    if (plane->inverse_transform)
        free_matrix(plane->inverse_transform);

    if (plane->translation_matrix)
        free_matrix(plane->translation_matrix);

    if (plane->rotation_matrix)
        free_matrix(plane->rotation_matrix);

    if (plane->scaling_matrix)
        free_matrix(plane->scaling_matrix);
}

void free_cylinder(t_cylinder *cylinder)
{
    if (cylinder->transform)
        free_matrix(cylinder->transform);

    if (cylinder->inverse_transform)
        free_matrix(cylinder->inverse_transform);

    if (cylinder->translation_matrix)
        free_matrix(cylinder->translation_matrix);

    if (cylinder->rotation_matrix)
        free_matrix(cylinder->rotation_matrix);

    if (cylinder->scaling_matrix)
        free_matrix(cylinder->scaling_matrix);
}

int	free_split(char **split)
{
	printf("inside free_split 0\n");
	int	i;

	if (split == NULL)
		return (0);

	i = 0;
	printf("inside free_split 1\n");
	while (split[i] != NULL)
	{
		free(split[i]);
		split[i] = NULL;
		printf("inside free_split %d\n", i);
		i++;
	}
	free(split);
	split = NULL;
	printf("returning 0 from free_split\n");
	return (0);
}

int	setup_data(t_element_count *element_count, t_map *map)
{
	printf("inside setup_data\n");
	map->element_count = element_count;
	printf("end of setup_data\n");
	return (1);
}

int	print_data(t_map *map)
{
	// Print Ambient
	if (map->ambient)
	{
		printf("Ambient:\n");
		printf("  Ratio: %f\n", map->ambient->ratio);
		printf("  t_color: (R: %d, G: %d, B: %d)\n", map->ambient->r, map->ambient->g, map->ambient->b);
	}

	// Print Camera List
	t_camera *camera = map->camera;
	while (camera)
	{
		printf("Camera:\n");
		printf("  Position: (%f, %f, %f)\n", camera->x, camera->y, camera->z);
		printf("  Orientation: (%f, %f, %f)\n", camera->nx, camera->ny, camera->nz);
		printf("  FOV: %f\n", camera->fov);
		camera = camera->next;
	}

	// Print Light List
	t_lights *lights = map->lights;
	while (lights)
	{
		printf("Light:\n");
		printf("  Position: (%f, %f, %f)\n", lights->x, lights->y, lights->z);
		printf("  Ratio: %f\n", lights->ratio);
		printf("  t_color: (R: %d, G: %d, B: %d)\n", lights->r, lights->g, lights->b);
		lights = lights->next;
	}

	// Print Sphere List
	t_spheres *sphere = map->spheres;
	while (sphere)
	{
		printf("Sphere:\n");
		printf("  Position: (%f, %f, %f)\n", sphere->x, sphere->y, sphere->z);
		printf("  Diameter: %f\n", sphere->diameter);
		printf("  t_color: (R: %d, G: %d, B: %d)\n", sphere->r, sphere->g, sphere->b);
		sphere = sphere->next;
	}

	// Print Plane List
	t_planes *plane = map->planes;
	while (plane)
	{
		printf("Plane:\n");
		printf("  Position: (%f, %f, %f)\n", plane->x, plane->y, plane->z);
		printf("  Normal: (%f, %f, %f)\n", plane->nx, plane->ny, plane->nz);
		printf("  t_color: (R: %d, G: %d, B: %d)\n", plane->r, plane->g, plane->b);
		plane = plane->next;
	}

	// Print Cylinder List
	t_cylinders *cylinder = map->cylinders;
	while (cylinder)
	{
		printf("Cylinder:\n");
		printf("  Position: (%f, %f, %f)\n", cylinder->x, cylinder->y, cylinder->z);
		printf("  Normal: (%f, %f, %f)\n", cylinder->nx, cylinder->ny, cylinder->nz);
		printf("  Diameter: %f\n", cylinder->diameter);
		printf("  Height: %f\n", cylinder->height);
		printf("  t_color: (R: %d, G: %d, B: %d)\n", cylinder->r, cylinder->g, cylinder->b);
		cylinder = cylinder->next;
	}

	// Print Element Count
	if (map->element_count)
	{
		printf("Element Count:\n");
		printf("  Ambients: %d\n", map->element_count->ambient);
		printf("  Cameras: %d\n", map->element_count->camera);
		printf("  Lights: %d\n", map->element_count->light);
		printf("  Spheres: %d\n", map->element_count->sphere);
		printf("  Planes: %d\n", map->element_count->plane);
		printf("  Cylinders: %d\n", map->element_count->cylinder);
	}

	return 0;
}

int terminate_data(t_map *map, t_var *var, char *error)
{
	t_camera *camera;
    t_lights *lights;
    t_camera *next_camera;
    t_lights *next_light;
    t_spheres *sphere;
    t_spheres *next_sphere;
    t_planes *plane;
    t_planes *next_plane;
    t_cylinders *cylinder;
    t_cylinders *next_cylinder;

    if (error)
        printf("%s\n", error);
    if (map->ambient)
		free(map->ambient);
	camera = map->camera;
    while (camera)
    {
        next_camera = camera->next;
        free(camera);
        camera = next_camera;
    }
	lights = map->lights;
    while (lights)
    {
        next_light = lights->next;
        free(lights);
        lights = next_light;
    }
    sphere = map->spheres;
    while (sphere)
    {
        next_sphere = sphere->next;
        free(sphere);
        sphere = next_sphere;
    }
    plane = map->planes;
    while (plane)
    {
        next_plane = plane->next;
        free(plane);
        plane = next_plane;
    }
    cylinder = map->cylinders;
    while (cylinder)
    {
        next_cylinder = cylinder->next;
        free(cylinder);
        cylinder = next_cylinder;
    }
    free(map);
    // Free spheres
    if (var->test_sphere)
    {
        for (int i = 0; i < var->num_spheres; i++)
        {
            free_sphere(&(var->test_sphere[i]));
        }
        free(var->test_sphere);
        var->test_sphere = NULL; // Avoid dangling pointer
    }
    // Free planes
    if (var->test_plane)
    {
        for (int i = 0; i < var->num_planes; i++)
        {
            free_plane(&(var->test_plane[i]));
        }
        free(var->test_plane);
        var->test_plane = NULL;
    }
    // Free cylinders
    if (var->test_cylinder)
    {
        for (int i = 0; i < var->num_cylinders; i++)
        {
            free_cylinder(&(var->test_cylinder[i]));
        }
        free(var->test_cylinder);
        var->test_cylinder = NULL;
    }
    return (1);
}
