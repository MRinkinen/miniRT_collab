#include "../includes/minirt.h"
#include "../includes/parsing.h"

// Function to create a light with intensity scaled by brightness
t_light light_create(t_tuple position, t_color intensity, float brightness)
{
    t_light light;

    light.position = position;
    light.color = multiply_color_scalar(intensity, brightness);
    light.brightness = brightness;
    return (light);
}

bool is_in_shadow(const t_tuple *point, const t_light *light, const t_object *objects, int num_objects)
{
    t_tuple light_dir;
    t_tuple shadow_origin; // Add small bias to avoid self-intersection
    t_ray shadow_ray;
    float threshold;
    float light_distance;
    int i;

    i = 0;
    light_dir = normalize(tuple_subtract(light->position, *point));
    shadow_origin = tuple_add(*point, tuple_multiply(light_dir, 0.01f)); // Adjust bias as needed
    shadow_ray = ray(shadow_origin, light_dir);

    light_distance = magnitude(tuple_subtract(light->position, shadow_origin));  // Distance from point to light source

    while (i < num_objects)
    {
        if (intersect_object(&shadow_ray, &objects[i], &threshold) && threshold > 0.001f && threshold < light_distance)
        {
            return (true);
        }
        i++;
    }
    return (false);
}

/*
static int sphere_shadow(const t_ray *shadow_ray, const t_object *objects, int num_objects)
{
    int i;
    float threshold;

    i = 0;
    while (i < num_objects)
    {
        if (objects[i].type == SPHERE && intersect_sphere(shadow_ray, &objects[i], &threshold) > 0.0f && threshold > 0.001f)
        {
            return (1);  // In shadow
        }
        i++;
    }
    return (0);  // No shadow from spheres
}

static int cylinder_shadow(const t_ray *shadow_ray, const t_object *objects, int num_objects)
{
    int i;
    float threshold;

    i = 0;
    while (i < num_objects)
    {
        if (objects[i].type == CYLINDER && intersect_cylinder(shadow_ray, &objects[i], &threshold) > 0.0f && threshold > 0.001f)
        {
            return (1);  // In shadow
        }
        i++;
    }
    return (0);  // No shadow from cylinders
}

static int plane_shadow(const t_ray *shadow_ray, const t_object *objects, int num_objects)
{
    int i;
    float threshold;

    i = 0;
    while (i < num_objects)
    {
        if (objects[i].type == PLANE && intersect_plane(shadow_ray, &objects[i], &threshold) > 0.0f && threshold > 0.001f)
        {
            return (1);  // In shadow
        }
        i++;
    }
    return (0);  // No shadow from planes
}

bool is_in_shadow(const t_tuple *point, const t_light *light, const t_object *objects, int num_objects)
{
    t_tuple light_dir;
    t_tuple shadow_origin;
    t_ray shadow_ray;
    int in_shadow;

    // Initialize the light direction and shadow ray origin with bias
    light_dir = normalize(tuple_subtract(light->position, *point));
    shadow_origin = tuple_add(*point, tuple_multiply(light_dir, 0.001f));
    //light_dir = normalize(tuple_subtract(*point, light->position));
    //shadow_origin = tuple_add(*point, tuple_multiply(light_dir, 0.001f));
    shadow_origin.x = point->x;
    shadow_origin.y = point->y;
    shadow_origin.z = point->z;
    shadow_origin.w = point->w;
    shadow_ray = ray(shadow_origin, light_dir);
    in_shadow = 0;

    // Check for shadow from spheres
    if (sphere_shadow(&shadow_ray, objects, num_objects))
        return true;

    // Check for shadow from cylinders
    if (cylinder_shadow(&shadow_ray, objects, num_objects))
        return true;

    // Check for shadow from planes
    if (plane_shadow(&shadow_ray, objects, num_objects))
        return true;

    // Return false if no shadows found
    return false;
}
*/

/*
bool is_in_shadow(const t_tuple *point, const t_light *light, const t_object *objects, int num_objects)
{
    t_tuple light_dir;
    t_tuple shadow_origin; // Add small bias to avoid self-intersection
    t_ray shadow_ray;
    float threshold;
    int i;

    i = 0;
    light_dir = normalize(tuple_subtract(light->position, *point));
    shadow_origin = tuple_add(*point, tuple_multiply(light_dir, 0.001f));
    shadow_ray = ray(shadow_origin, light_dir);
    while (i < num_objects)
    {
        if (intersect_object(&shadow_ray, &objects[i], &threshold) && threshold > 0.001f)
        {
            return (true);
        }
        i++;
    }
    return (false);
}*/

// // Function to calculate Phong lighting
// t_color calculate_phong_lighting(t_var *var, const t_tuple *point, const t_tuple *normal, const t_tuple *view_dir)
// {
//     float spec;
//     float diff_amount;
//     t_tuple light_dir;
//     t_tuple reflect_dir;
//     t_color ambient;
//     t_color diffuse;
//     t_color specular;
//     t_color final_color;

//     ambient = multiply_color_scalar(var->pointlights[0].color, 0.1f); // Ambient component
//     light_dir = normalize(tuple_subtract(var->pointlights[0].position, *point));
//     diff_amount = fmax(dot(*normal, light_dir), 0.0f);
//     diffuse = multiply_color_scalar(var->pointlights[0].color, diff_amount); // Diffuse component
//     reflect_dir = tuple_subtract(tuple_multiply(*normal, 2 * dot(*normal, light_dir)), light_dir);
//     spec = pow(fmax(dot(*view_dir, reflect_dir), 0.0f), 32);
//     specular = multiply_color_scalar(var->pointlights[0].color, spec); // Specular component
//     final_color = multiply_colors(var->temp_color, ambient);
//     if (!is_in_shadow(point, &var->pointlights[0], var->objects, var->num_objects))
//     {
//         final_color = color_add(final_color, multiply_colors(var->temp_color, diffuse));
//         final_color = color_add(final_color, specular);
//     }
//     return (final_color);
// }

t_color calculate_phong_lighting(t_var *var, const t_tuple *point, const t_tuple *normal, const t_tuple *view_dir)
{
    float spec;
    float diff_amount;
    t_tuple light_dir;
    t_tuple reflect_dir;
    t_color ambient;
    t_color diffuse;
    t_color specular;
    t_color final_color;

    // Use global ambient settings
    ambient = var->ambientl; // Adjusted Ambient component
    light_dir = normalize(tuple_subtract(var->pointlights[0].position, *point));
    diff_amount = fmax(dot(*normal, light_dir), 0.0f);
    diffuse = multiply_color_scalar(var->pointlights[0].color, diff_amount); // Diffuse component
    reflect_dir = tuple_subtract(tuple_multiply(*normal, 2 * dot(*normal, light_dir)), light_dir);
    spec = pow(fmax(dot(*view_dir, reflect_dir), 0.0f), 32); // Use var->shininess for flexibility
    specular = multiply_color_scalar(var->pointlights[0].color, spec); // Specular component
    final_color = multiply_colors(var->temp_color, ambient);
    if (!is_in_shadow(point, &var->pointlights[0], var->objects, var->num_objects))
    {
        final_color = color_add(final_color, multiply_colors(var->temp_color, diffuse));
        final_color = color_add(final_color, specular);
    }
    return (final_color);
}
