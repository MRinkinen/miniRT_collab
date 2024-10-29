/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:22:00 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/29 12:04:37 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_matrix	*inverse(t_matrix *m)
{
	float		det;
	t_matrix	*cofactor_m;
	t_matrix	*adjugate_m;
	t_matrix	*inverse_m;

	det = determinant(m);
	if (det == 0.0f)
	{
		printf("Matrix is not invertible.\n");
		exit(1);
	}
	cofactor_m = cofactor_matrix(m);
	adjugate_m = transpose(cofactor_m);
	inverse_m = (t_matrix *)malloc(sizeof(t_matrix));
	if (!inverse_m)
		return (NULL);
	inverse_m->rows = m->rows;
	inverse_m->cols = m->cols;
	fill_inverse(inverse_m, adjugate_m, det);
	free(cofactor_m);
	free(adjugate_m);
	return (inverse_m);
}

t_matrix	*cofactor_matrix(const t_matrix *m)
{
	int			i;
	int			j;
	t_matrix	*cofactor_m;

	cofactor_m = (t_matrix *)malloc(sizeof(t_matrix));
	if (cofactor_m == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	cofactor_m->rows = m->rows;
	cofactor_m->cols = m->cols;
	i = 0;
	while (i < m->rows)
	{
		j = 0;
		while (j < m->cols)
		{
			cofactor_m->data[i][j] = cofactor(m, i, j);
			j++;
		}
		i++;
	}
	return (cofactor_m);
}

t_tuple	get_default_normal(void)
{
	t_tuple	default_normal;

	default_normal = vector(0, 1, 0);
	return (normalize(default_normal));
}

bool	check_special_case(t_tuple v2)
{
	if (fabs(v2.x - 1.0) < EPSILON && fabs(v2.y) < \
	EPSILON && fabs(v2.z) < EPSILON)
		return (true);
	if (fabs(v2.z - 1.0) < EPSILON && fabs(v2.x) < \
	EPSILON && fabs(v2.y) < EPSILON)
		return (true);
	return (false);
}

t_tuple	get_arbitrary_axis(t_tuple v1)
{
	t_tuple	arbitrary_axis;

	arbitrary_axis = vector(1, 0, 0);
	if (fabs(v1.x) > fabs(v1.y))
		arbitrary_axis = vector(0, 1, 0);
	return (arbitrary_axis);
}
