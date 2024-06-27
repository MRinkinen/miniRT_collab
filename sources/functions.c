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

