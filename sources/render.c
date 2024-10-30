/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:07:42 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 16:12:24 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include <pthread.h>

pthread_mutex_t g_write_color_mutex;
pthread_mutex_t g_shade_color_mutex;

typedef struct s_thread_data
{
	t_var *var;
	int start_y;
	int end_y;
} t_thread_data;

void	write_color(t_color col, t_var *var, int x, int y)
{
	int	color;

	color = ft_pixel(col.r, col.g, col.b, 255);
	mlx_put_pixel(var->screenimage, x, y, color);
}

t_color	shade_pixel(t_var *var, t_ray *r, \
t_object *closest_object, float closest_t)
{
	t_tuple	intersection_point;
	t_tuple	normal;
	t_color	object_color;
	t_tuple	view_dir;

	intersection_point = tuple_add(r->origin, \
	tuple_multiply(r->direction, closest_t));
	normal = calculate_normal(closest_object, &intersection_point);
	view_dir = normalize(tuple_subtract(var->cam.position, intersection_point));
	if (closest_object->type == SPHERE)
		object_color = closest_object->data.sphere.color;
	else if (closest_object->type == CYLINDER)
		object_color = closest_object->data.cylinder.color;
	else if (closest_object->type == PLANE)
		object_color = closest_object->data.plane.color;
	else
		object_color = (t_color){0, 0, 0};
	var->temp_color = object_color;
	return (calculate_phong_lighting(var, \
	&intersection_point, &normal, &view_dir));
}

void	process_pixel(t_var *var, int x, int y)
{
	t_ray		r;
	t_color		pixel_color;
	t_object	*closest_object;
	float		closest_t;

	closest_object = NULL;
	pixel_color = var->ambientl;
	r.px_center = tuple_add(var->cam.loc_00, \
	tuple_multiply(var->cam.delta_u, x));
	r.px_center = tuple_add(r.px_center, tuple_multiply(var->cam.delta_v, y));
	r.direction = normalize(tuple_subtract(r.px_center, var->cam.position));
	if (dot(r.direction, var->cam.forward) < 0)
		r.direction = tuple_multiply(r.direction, -1.0);
	r = ray(var->cam.position, r.direction);
	if (find_closest_intersection(var, &r, &closest_object, &closest_t))
	{
		pthread_mutex_lock(&g_shade_color_mutex);
		pixel_color = shade_pixel(var, &r, closest_object, closest_t);
		pthread_mutex_unlock(&g_shade_color_mutex);
	}
	pthread_mutex_lock(&g_write_color_mutex);
	write_color(pixel_color, var, x, y);
	pthread_mutex_unlock(&g_write_color_mutex);
}

void *render_section(void *arg)
{
	t_thread_data *data = (t_thread_data *)arg;
	t_var *var = data->var;
	int y, x;

	y = data->start_y;
	while (y < data->end_y)
	{
		x = 0;
		while (x < WIDTH)
		{
			process_pixel(var, x, y);
			++x;
		}
		++y;
	}
	return (NULL);
}

void printimage(void *param)
{
	t_var *var = (t_var *)param;
	int num_threads = 8; // Number of threads to use (adjust based on your system)
	pthread_t threads[num_threads];
	t_thread_data thread_data[num_threads];
	int rows_per_thread = HEIGHT / num_threads;
	int i;

	pthread_mutex_init(&g_write_color_mutex, NULL);
	pthread_mutex_init(&g_shade_color_mutex, NULL);
	i = 0;
	while (i < num_threads)
	{
		thread_data[i].var = var;
		thread_data[i].start_y = i * rows_per_thread;
		thread_data[i].end_y = (i == num_threads - 1) ? HEIGHT : (i + 1) * rows_per_thread;
		pthread_create(&threads[i], NULL, render_section, &thread_data[i]);
		++i;
	}
	i = 0;
	while (i < num_threads)
	{
		pthread_join(threads[i], NULL);
		++i;
	}
	pthread_mutex_destroy(&g_write_color_mutex);
	pthread_mutex_destroy(&g_shade_color_mutex);
}
