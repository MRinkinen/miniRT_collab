#include "../includes/minirt.h"
#include "../includes/parsing.h"

t_light *point_light(t_tuple position, t_color intensity, float brightness)
{
    t_light *light = malloc(sizeof(t_light));
    if (!light) {
        // Handle allocation failure
        return NULL;
    }
    light->position = position;
    // Scale the intensity by the brightness ratio
    light->intensity = multiply_color_scalar(intensity, brightness);
    light->brightness = brightness;
    return (light);
}

t_light light_create(t_tuple position, t_color intensity)
{
    t_light light;
    light.position = position;
    light.intensity = intensity;
    return light;
}

bool is_in_shadow(const t_tuple *point, const t_light *light, const t_object *objects, int num_objects)
{
    t_tuple light_dir = normalize(tuple_subtract(light->position, *point));
    t_tuple shadow_origin = tuple_add(*point, tuple_multiply(light_dir, 0.001f)); // Add small bias to avoid self-intersection
    t_ray shadow_ray = {shadow_origin, light_dir};
    float t;

    for (int i = 0; i < num_objects; i++)
    {
        if (intersect_object(&shadow_ray, &objects[i], &t) && t > 0.001f)
        {
            return true;
        }
    }

    return false;
}

t_color calculate_phong_lighting(const t_tuple *point, const t_tuple *normal, const t_light *light, const t_color *object_color, const t_tuple *view_dir, const t_object *objects, int num_objects)
{

    t_color ambient = multiply_color_scalar(light->intensity, 0.1f); // Ambient component

    t_tuple light_dir = normalize(tuple_subtract(light->position, *point));
    float diff = fmax(dot(*normal, light_dir), 0.0f);
    t_color diffuse = multiply_color_scalar(light->intensity, diff); // Diffuse component

    t_tuple reflect_dir = tuple_subtract(tuple_multiply(*normal, 2 * dot(*normal, light_dir)), light_dir);
    float spec = pow(fmax(dot(*view_dir, reflect_dir), 0.0f), 32);
    t_color specular = multiply_color_scalar(light->intensity, spec); // Specular component

    t_color final_color = multiply_colors(*object_color, ambient);
     if (!is_in_shadow(point, light, objects, num_objects))
     {
        final_color = color_add(final_color, multiply_colors(*object_color, diffuse));
        final_color = color_add(final_color, specular);
    }
    return final_color;
}
