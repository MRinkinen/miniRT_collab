/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:33:18 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/26 16:53:36 by tvalimak         ###   ########.fr       */
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

// Function to create an identity matrix
t_matrix	*identity_matrix()
{
	int			i;
	int			j;
	t_matrix	*identity;

	identity = (t_matrix *)malloc(sizeof(t_matrix));
	if (identity == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	identity->rows = 4;
	identity->cols = 4;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			if (i == j)
				identity->data[i][j] = 1.0;
			else
				identity->data[i][j] = 0.0;
		}
	}
	return (identity);
}

// Function to transpose a matrix
t_matrix*	transpose(t_matrix *m)
{
	int			i;
	int			j;
	t_matrix	*transposed;

	transposed = (t_matrix *)malloc(sizeof(t_matrix));
	if (transposed == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	transposed->rows = m->cols;
	transposed->cols = m->rows;
	i = 0;
	while (i < m->rows)
	{
		j = 0;
		while (j < m->cols)
		{
			transposed->data[j][i] = m->data[i][j];
			j++;
		}
		i++;
	}
	return (transposed);
}

void multiply_elements(t_matrix *result, t_matrix *a, t_matrix *b)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result->data[i][j] = 0;
			k = 0;
			while (k < 4)
			{
				result->data[i][j] += a->data[i][k] * b->data[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

t_matrix *t_matrix_multiply(t_matrix *a, t_matrix *b)
{
	t_matrix	*result;

	if (a->rows != 4 || a->cols != 4 || b->rows != 4 || b->cols != 4)
	{
		printf("Error: Both matrices must be 4x4.\n");
		return (NULL);
	}
	result = (t_matrix *)malloc(sizeof(t_matrix));
	if (result == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	result->rows = 4;
	result->cols = 4;
	multiply_elements(result, a, b);
	return (result);
}

// Function to create a 4x4 matrix from an array of values
t_matrix *create_4x4_matrix(float values[16])
{
	int				row;
	int				col;
	t_matrix		*matrix;

	matrix = (t_matrix *)malloc(sizeof(t_matrix));
	if (matrix == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	matrix->rows = 4;
	matrix->cols = 4;
	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			matrix->data[row][col] = values[row * 4 + col];
			col++;
		}
		row++;
	}
	return (matrix);
}
