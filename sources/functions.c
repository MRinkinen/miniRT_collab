#include "../includes/minirt.h"

//using color = t_vec3;

void write_color(t_vec3 vec, t_var *var, int x, int y)
{
    //int color = ft_pixel(vector3->e[0],vector3->e[1],vector3->e[2],255);
    double r = vec.e[0];
    double g = vec.e[1];
    double b = vec.e[2];

    // Translate the [0,1] component values to the byte range [0,255].
    int ir = (int)(255.999 * r);
    int ig = (int)(255.999 * g);
    int ib = (int)(255.999 * b);

    int color = ft_pixel(ir,ig,ib,255);
	mlx_put_pixel(var->testimage, x, y, color);
}



// Assuming t_color is defined similar to t_vec3 for color representation


// Utility functions for vector operations, assuming these are defined elsewhere
t_vec3 vec3_subtract(t_vec3 a, t_vec3 b);
double vec3_dot(t_vec3 a, t_vec3 b);
t_vec3 vec3_multiply_scalar(t_vec3 v, double s);
t_vec3 vec3_unit_vector(t_vec3 v);
t_color color(double r, double g, double b);

bool hit_sphere(const t_vec3 center, double radius, const t_ray r)
{
    t_vec3 oc = t_vec3_subtract_vectors(&r.orig, &center);
    double a = t_vec3_dot(&r.dir, &r.dir);
    double b = 2.0 * t_vec3_dot(&r.dir, &oc);
    double c = t_vec3_dot(&oc, &oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}

// t_color ray_color(const t_ray r)
// {
//     if (hit_sphere((t_vec3){{0, 0, -1}}, 0.5, r))
//         return color(1, 0, 0);

//     t_vec3 unit_direction = vec3_unit_vector(r.dir);
//     double t = 0.5 * (unit_direction.e[1] + 1.0);
//     t_color white = color(1.0, 1.0, 1.0);
//     t_color blue = color(0.5, 0.7, 1.0);
//     t_color result = {
//         .e = {
//             (1.0 - t) * white.e[0] + t * blue.e[0],
//             (1.0 - t) * white.e[1] + t * blue.e[1],
//             (1.0 - t) * white.e[2] + t * blue.e[2]
//         }
//     };
//     return result;
// }

