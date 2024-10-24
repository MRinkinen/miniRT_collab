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

bool is_in_shadow(t_hit *hit, const t_light *light, const t_object *objects, int num_objects)
{
    t_tuple light_dir;
    t_tuple shadow_origin; // Add small bias to avoid self-intersection
    t_ray shadow_ray;
    float threshold;
    int i;

    i = 0;
    light_dir = normalize(tuple_subtract(light->position, hit->point));
    shadow_origin = tuple_add(hit->point, tuple_multiply(light_dir, 0.001f));
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
}

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

t_color calculate_phong_lighting(t_var *var, t_hit *hit, const t_tuple *view_dir)
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
    light_dir = normalize(tuple_subtract(var->pointlights[0].position, hit->point));
    diff_amount = fmax(dot(hit->normal, light_dir), 0.0f);
    diffuse = multiply_color_scalar(var->pointlights[0].color, diff_amount); // Diffuse component
    reflect_dir = tuple_subtract(tuple_multiply(hit->normal, 2 * dot(hit->normal, light_dir)), light_dir);
    spec = pow(fmax(dot(*view_dir, reflect_dir), 0.0f), 32); // Use var->shininess for flexibility
    specular = multiply_color_scalar(var->pointlights[0].color, spec); // Specular component
    final_color = multiply_colors(var->temp_color, ambient);
    if (!is_in_shadow(hit, &var->pointlights[0], var->objects, var->num_objects))
    {
        final_color = color_add(final_color, multiply_colors(hit->color, diffuse));
        final_color = color_add(final_color, specular);
    }
    return (final_color);
}
