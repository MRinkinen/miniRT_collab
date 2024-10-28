/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:20:42 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:30:10 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"
#include "../includes_bonus/parsing_bonus.h"

void	cylinder_helper(t_object *object, t_tuple center, \
t_tuple orientation, float radius)
{
	t_matrix	*temp;

	object->type = CYLINDER;
	object->data.cylinder.translation_matrix = \
	translation(center.x, center.y, center.z);
	object->data.cylinder.rotation_matrix = rotation_from_normal(orientation);
	object->data.cylinder.scaling_matrix = scaling(radius, 1.0f, radius);
	temp = t_matrix_multiply(object->data.cylinder.scaling_matrix, \
	object->data.cylinder.rotation_matrix);
	object->data.cylinder.transform = t_matrix_multiply(temp, \
	object->data.cylinder.translation_matrix);
	free(temp);
	object->data.cylinder.inverse_transform = \
	inverse(object->data.cylinder.transform);
}

void	create_cylinders(t_var *var, t_map *map, int *obj_index)
{
	t_cylinders	*current_cylinder;
	t_object	*object;
	t_tuple		orientation;
	t_tuple		center;
	float		radius;

	current_cylinder = map->cylinders;
	while (current_cylinder != NULL)
	{
		object = &var->objects[*obj_index];
		orientation = normalize(vector(current_cylinder->nx, \
		current_cylinder->ny, current_cylinder->nz));
		center = point(current_cylinder->x, current_cylinder->y, \
		current_cylinder->z);
		radius = current_cylinder->diameter / 2.0f;
		object->data.cylinder.center = center;
		object->data.cylinder.radius = radius;
		object->data.cylinder.height = current_cylinder->height;
		object->data.cylinder.orientation = orientation;
		object->data.cylinder.color = t_color_create(current_cylinder->r, \
		current_cylinder->g, current_cylinder->b);
		cylinder_helper(object, center, orientation, radius);
		current_cylinder = current_cylinder->next;
		(*obj_index)++;
	}
}

t_tuple	calculate_cylinder_normal(t_cylinder *cylinder, t_tuple *point)
{
	t_tuple	btm_cent;
	t_tuple	top_cent;
	t_tuple	surf_normal;
	double	t;
	t_tuple	local_point;

	cylinder->orientation = normalize(cylinder->orientation);
	btm_cent = tuple_subtract(cylinder->center, \
	tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	top_cent = tuple_add(cylinder->center, \
	tuple_multiply(cylinder->orientation, cylinder->height / 2.0));
	if (magnitude(tuple_subtract(*point, top_cent)) < cylinder->radius)
		return (cylinder->orientation);
	if (magnitude(tuple_subtract(*point, btm_cent)) < cylinder->radius)
		return (tuple_multiply(cylinder->orientation, -1));
	t = dot(tuple_subtract(*point, btm_cent), cylinder->orientation);
	local_point = tuple_add(btm_cent, tuple_multiply(cylinder->orientation, t));
	surf_normal = normalize(tuple_subtract(*point, local_point));
	return (surf_normal);
}

void	set_cylinder_params(const t_ray *ray, const t_cylinder *cylinder, \
t_cylinder_params *params)
{
	params->oc = tuple_subtract(cylinder->center, ray->origin);
	params->direction = ray->direction;
	params->axis = cylinder->orientation;
	params->radius = cylinder->radius;
}
