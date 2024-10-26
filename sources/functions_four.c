/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_four.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:27:36 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/26 18:28:41 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

float	minor(const t_matrix *m, int row, int col)
{
	t_matrix	*sub_m;
	float		det;

	sub_m = submatrix(m, row, col);
	det = determinant(sub_m);
	free(sub_m);
	return (det);
}

float	cofactor(const t_matrix *m, int row, int col)
{
	float	minor_value;

	minor_value = minor(m, row, col);
	if ((row + col) % 2 != 0)
	{
		return (-minor_value);
	}
	return (minor_value);
}

t_tuple	matrix_to_tuple(t_matrix *m)
{
	return (tuple(m->data[0][0], m->data[1][0], m->data[2][0], m->data[3][0]));
}

t_matrix	*translation(float x, float y, float z)
{
	t_matrix	*transform;

	transform = identity_matrix();
	transform->data[0][3] = x;
	transform->data[1][3] = y;
	transform->data[2][3] = z;

	return (transform);
}

t_matrix	*allocate_submatrix(int new_rows, int new_cols)
{
	t_matrix	*sub_m;

	sub_m = (t_matrix *)malloc(sizeof(t_matrix));
	if (sub_m == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	sub_m->rows = new_rows;
	sub_m->cols = new_cols;
	return (sub_m);
}
