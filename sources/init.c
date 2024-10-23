#include "../includes/minirt.h"
#include "../includes/parsing.h"

void initialize_scene(t_var *var, t_map *map)
{
    int obj_index = 0;

    var->num_objects = map->element_count->sphere + map->element_count->cylinder + map->element_count->plane;
    var->objects = malloc(var->num_objects * sizeof(t_object));
    if (var->objects == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Initialize spheres
    t_spheres *current_sphere = map->spheres;
    while (current_sphere != NULL)
    {
        t_tuple center = point(current_sphere->x, current_sphere->y, current_sphere->z);
        float radius = current_sphere->diameter / 2.0f; // Assuming diameter is provided
        t_color color = t_color_create(current_sphere->r, current_sphere->g, current_sphere->b);

        var->objects[obj_index].type = SPHERE;
        var->objects[obj_index].data.sphere.center = center;
        var->objects[obj_index].data.sphere.radius = radius;
        var->objects[obj_index].data.sphere.color = color;

        current_sphere = current_sphere->next;
        printf("Sphere center: %f %f %f\n", center.x, center.y, center.z);
        obj_index++;
    }

    // Initialize cylinders
    t_cylinders *current_cylinder = map->cylinders;
    while (current_cylinder != NULL)
    {
        printf("Inside cylinder loop 1 \n");
        t_tuple orientation = normalize(vector(current_cylinder->nx, current_cylinder->ny, current_cylinder->nz));
        t_tuple center = point(current_cylinder->x, current_cylinder->y, current_cylinder->z);
        float radius = current_cylinder->diameter / 2.0f; // Assuming diameter is provided
        float height = current_cylinder->height;
        t_color color = t_color_create(current_cylinder->r, current_cylinder->g, current_cylinder->b);
        printf("radius %f \n", radius);
        printf("height %f \n", height);

        //print_matrix(var->objects[obj_index].data.cylinder.transform);
        //print_matrix(var->objects[obj_index].data.cylinder.inverse_transform);

        printf("Inside cylinder loop 2 \n");
        // Initialize transformation matrices
        var->objects[obj_index].data.cylinder.translation_matrix = translation(center.x, center.y, center.z);
        var->objects[obj_index].data.cylinder.rotation_matrix = rotation_from_normal(orientation);
        var->objects[obj_index].data.cylinder.scaling_matrix = scaling(radius, 1.0f, radius); // Scale by radius in xz and height in y

        printf("Inside cylinder loop 3 \n");
        // Combine transformations into one matrix
        var->objects[obj_index].data.cylinder.transform = t_matrix_multiply(t_matrix_multiply(var->objects[obj_index].data.cylinder.scaling_matrix, var->objects[obj_index].data.cylinder.rotation_matrix), var->objects[obj_index].data.cylinder.translation_matrix);
        //var->objects[obj_index].data.cylinder.transform = t_matrix_multiply(t_matrix_multiply(var->objects[obj_index].data.cylinder.translation_matrix, var->objects[obj_index].data.cylinder.rotation_matrix), var->objects[obj_index].data.cylinder.scaling_matrix);

        printf("Inside cylinder loop 5 \n");
        // Calculate the inverse transform for ray-cylinder intersection calculations
        var->objects[obj_index].data.cylinder.inverse_transform = inverse(var->objects[obj_index].data.cylinder.transform);

        printf("Inside cylinder loop 6 \n");
        var->objects[obj_index].type = CYLINDER;
        var->objects[obj_index].data.cylinder.center = center;
        var->objects[obj_index].data.cylinder.radius = radius;
        var->objects[obj_index].data.cylinder.height = height;
        var->objects[obj_index].data.cylinder.color = color;
        var->objects[obj_index].data.cylinder.orientation = orientation;

        current_cylinder = current_cylinder->next;
        obj_index++;
    }
    // Initialize planes
    t_planes *current_plane = map->planes;
    while (current_plane != NULL)
    {
        t_tuple orientation = normalize(vector(current_plane->nx, current_plane->ny, current_plane->nz));
        t_tuple center = point(current_plane->x, current_plane->y, current_plane->z);
        t_tuple normal = vector(current_plane->nx, current_plane->ny, current_plane->nz);
        t_color color = t_color_create(current_plane->r, current_plane->g, current_plane->b);

        var->objects[obj_index].type = PLANE;
        var->objects[obj_index].data.plane.center;
        var->objects[obj_index].data.plane.point = center;
        var->objects[obj_index].data.plane.normal = normal;
        var->objects[obj_index].data.plane.color = color;

        current_plane = current_plane->next;
        obj_index++;
    }
}

void init_light(t_var *var, t_map *map)
{
    int i = 0;
    t_lights *current_light = map->lights;
    var->num_lights = map->element_count->light;
    var->pointlights = malloc(var->num_lights * sizeof(t_light));
    if (!var->pointlights)
    {
        // Handle malloc failure (optional)
        return;
    }
    while (current_light != NULL)
    {
        t_tuple position = point(current_light->x, current_light->y, current_light->z);
        t_color intensity = t_color_create(current_light->r, current_light->b, current_light->g);
        float brightness = current_light->ratio;
        var->pointlights[i] = light_create(position, intensity,brightness);
        //var->pointlights[i].direction = normalize((t_tuple){0.0f, -1.0f, 0.0f}); // Example direction pointing downwards
        //var->pointlights[i].cutoff_angle = 30.0f; // Example cutoff angle in degrees
        i++;
        current_light = current_light->next;
    }
}

void init_ambient_color(t_var *var, t_map *map)
{
    t_color ambient = t_color_create(map->ambient->r,map->ambient->b,map->ambient->g);
    var->ambientl = multiply_color_scalar(ambient,map->ambient->ratio);
}
