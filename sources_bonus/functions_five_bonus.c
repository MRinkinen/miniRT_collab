/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_five_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:29:04 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/28 11:30:38 by mrinkine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minirt_bonus.h"

void	fill_submatrix(t_matrix *sub_m, const t_matrix *m, \
int remove_row, int remove_col)
{
	int	i;
	int	j;
	int	row_offset;
	int	col_offset;

	i = 0;
	while (i < sub_m->rows)
	{
		if (i >= remove_row)
			row_offset = 1;
		else
			row_offset = 0;
		j = 0;
		while (j < sub_m->cols)
		{
			if (j >= remove_col)
				col_offset = 1;
			else
				col_offset = 0;
			sub_m->data[i][j] = m->data[i + row_offset][j + col_offset];
			j++;
		}
		i++;
	}
}

t_matrix	*submatrix(const t_matrix *m, int remove_row, int remove_col)
{
	int			new_rows;
	int			new_cols;
	t_matrix	*sub_m;

	new_rows = m->rows - 1;
	new_cols = m->cols - 1;
	sub_m = allocate_submatrix(new_rows, new_cols);
	if (sub_m == NULL)
		return (NULL);
	fill_submatrix(sub_m, m, remove_row, remove_col);
	return (sub_m);
}

t_matrix	*allocate_matrix(int rows, int cols)
{
	t_matrix	*m;

	m = (t_matrix *)malloc(sizeof(t_matrix));
	if (m == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	m->rows = rows;
	m->cols = cols;
	return (m);
}

void	initialize_identity(t_matrix *m)
{
	int	i;
	int	j;

	i = 0;
	while (i < m->rows)
	{
		j = 0;
		while (j < m->cols)
		{
			if (i == j)
				m->data[i][j] = 1.0f;
			else
				m->data[i][j] = 0.0f;
			j++;
		}
		i++;
	}
}

t_matrix	*scaling(float x, float y, float z)
{
	t_matrix	*transform;

	transform = allocate_matrix(4, 4);
	if (transform == NULL)
		return (NULL);
	initialize_identity(transform);
	transform->data[0][0] = x;
	transform->data[1][1] = y;
	transform->data[2][2] = z;
	return (transform);
}
