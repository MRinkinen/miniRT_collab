/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:25:57 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/27 19:43:37 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

void	hyperboloid_helper(t_object *object, t_tuple center, \
t_tuple orientation)
{
	t_matrix	*temp;

	object->type = HYPERBOLOID;
	object->data.hyperboloid.translation_matrix = \
		translation(center.x, center.y, center.z);
	object->data.hyperboloid.rotation_matrix = \
		rotation_from_normal(orientation);
	object->data.hyperboloid.scaling_matrix = \
		scaling(object->data.hyperboloid.a, object->data.hyperboloid.b, object->data.hyperboloid.c);

	// Combine scaling, rotation, and translation
	temp = t_matrix_multiply(object->data.hyperboloid.scaling_matrix, \
		object->data.hyperboloid.rotation_matrix);
	object->data.hyperboloid.transform = t_matrix_multiply(temp, \
		object->data.hyperboloid.translation_matrix);
	free(temp);

	// Calculate the inverse transform
	object->data.hyperboloid.inverse_transform = \
		inverse(object->data.hyperboloid.transform);
}

void	create_hyperboloids(t_var *var, t_map *map, int *obj_index)
{
	t_hyperboloids	*current_hyperboloid;
	t_object		*object;
	t_tuple			orientation;
	t_tuple			center;

	printf("in create hyper\n");
	current_hyperboloid = map->hyperboloids;
	while (current_hyperboloid != NULL)
	{
		object = &var->objects[*obj_index];
		orientation = normalize(vector(current_hyperboloid->nx, \
			current_hyperboloid->ny, current_hyperboloid->nz));
		center = point(current_hyperboloid->x, current_hyperboloid->y, \
			current_hyperboloid->z);
		object->data.hyperboloid.center = center;
		object->data.hyperboloid.height = current_hyperboloid->height;
		object->data.hyperboloid.orientation = orientation;
		object->data.hyperboloid.a = current_hyperboloid->aa;
		object->data.hyperboloid.b = current_hyperboloid->bb;
		object->data.hyperboloid.c = current_hyperboloid->cc;
		object->data.hyperboloid.color = t_color_create(current_hyperboloid->r, \
			current_hyperboloid->g, current_hyperboloid->b);

		// Call the helper to set up transformations
		hyperboloid_helper(object, center, orientation);

		current_hyperboloid = current_hyperboloid->next;
		(*obj_index)++;
	}
}


/*
void	hyperboloid_helper(t_object *object, t_tuple center, \
t_tuple orientation, float radius)
{
	t_matrix	*temp;

	object->type = HYPERBOLOID;
	object->data.hyperboloid.translation_matrix = \
	translation(center.x, center.y, center.z);
	object->data.hyperboloid.rotation_matrix = \
	rotation_from_normal(orientation);
	object->data.hyperboloid.scaling_matrix = scaling(object->\
	data.hyperboloid.a, object->data.hyperboloid.b, object->data.hyperboloid.c);
	temp = t_matrix_multiply(object->data.hyperboloid.scaling_matrix, \
	object->data.hyperboloid.rotation_matrix);
	object->data.hyperboloid.transform = t_matrix_multiply(temp, \
	object->data.hyperboloid.translation_matrix);
	free(temp);
	object->data.hyperboloid.inverse_transform = \
	inverse(object->data.hyperboloid.transform);
}*/

/*
void	create_hyperboloids(t_var *var, t_map *map, int *obj_index)
{
	t_hyperboloids	*current_hyperboloid;
	t_object		*object;
	t_tuple			orientation;
	t_tuple			center;
	float			radius;

	current_hyperboloid = map->hyperboloids;
	while (current_hyperboloid != NULL)
	{
		object = &var->objects[*obj_index];
		orientation = normalize(vector(current_hyperboloid->nx, \
		current_hyperboloid->ny, current_hyperboloid->nz));
		center = point(current_hyperboloid->x, current_hyperboloid->y, \
		current_hyperboloid->z);
		object->data.hyperboloid.center = center;
		object->data.hyperboloid.height = current_hyperboloid->height;
		object->data.hyperboloid.orientation = orientation;
		object->data.hyperboloid.a = current_hyperboloid->aa;
		object->data.hyperboloid.b = current_hyperboloid->bb;
		object->data.hyperboloid.c = current_hyperboloid->cc;
		object->data.hyperboloid.color = t_color_create(current_hyperboloid->r, \
		current_hyperboloid->g, current_hyperboloid->b);
		hyperboloid_helper(object, center, orientation, radius);
		current_hyperboloid = current_hyperboloid->next;
		(*obj_index)++;
	}
}*/
