/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:18 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/26 19:07:38 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// Function to return the determinant of a 2x2 matrix
float	determinant_2x2(const t_matrix *m)
{
	return (m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0]);
}

// Function to return the determinant of a 3x3 matrix
float	determinant_3x3(const t_matrix *m)
{
	return (m->data[0][0] * (m->data[1][1] * m->data[2][2] - \
			m->data[1][2] * m->data[2][1]) -
			m->data[0][1] * (m->data[1][0] * m->data[2][2] - \
			m->data[1][2] * m->data[2][0]) +
			m->data[0][2] * (m->data[1][0] * m->data[2][1] - \
			m->data[1][1] * m->data[2][0]));
}

float	determinant_4x4(const t_matrix *m)
{
	int		col;
	float	det;

	det = 0.0;
	col = 0;
	while (col < m->cols)
	{
		det += m->data[0][col] * cofactor(m, 0, col);
		col++;
	}
	return (det);
}

// Function to return the determinant of a matrix
float	determinant(const t_matrix *m)
{
	if (m->rows == 2 && m->cols == 2)
		return (determinant_2x2(m));
	if (m->rows == 3 && m->cols == 3)
		return (determinant_3x3(m));
	if (m->rows == 4 && m->cols == 4)
		return (determinant_4x4(m));
	printf("Determinant calculation not implemented \
	for matrices larger than 4x4.\n");
	return (0.0f);
}

void	fill_inverse(t_matrix *inverse_m, t_matrix *adjugate_m, float det)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < inverse_m->rows)
	{
		j = 0;
		while (j < inverse_m->cols)
		{
			inverse_m->data[i][j] = adjugate_m->data[i][j] / det;
			j++;
		}
		i++;
	}
}
