/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_two_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:19:53 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:31:08 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"

// Function to create an identity matrix
t_matrix	*identity_matrix(void)
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
t_matrix	*transpose(t_matrix *m)
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

void	multiply_elements(t_matrix *result, t_matrix *a, t_matrix *b)
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

t_matrix	*t_matrix_multiply(t_matrix *a, t_matrix *b)
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
t_matrix	*create_4x4_matrix(float values[16])
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
