/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 19:39:44 by tvalimak          #+#    #+#             */
/*   Updated: 2024/10/27 16:14:52 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include "./minirt.h"

# include <fcntl.h>
# include <stdio.h>

typedef struct s_map	t_map;
typedef struct s_var	t_var;

typedef struct s_ambient
{
	float				ratio;
	int					r;
	int					g;
	int					b;
	t_map				*map;
}				t_ambient;

typedef struct s_camera
{
	float			x;
	float			y;
	float			z;
	float			nx;
	float			ny;
	float			nz;
	float			fov;
	struct s_camera	*next;
	t_map			*map;
}				t_camera;

typedef struct s_lights
{
	float			x;
	float			y;
	float			z;
	float			ratio;
	int				r;
	int				g;
	int				b;
	struct s_lights	*next;
	t_map			*map;
}				t_lights;

typedef struct s_spheres
{
	float				x;
	float				y;
	float				z;
	float				diameter;
	int					r;
	int					g;
	int					b;
	struct s_spheres	*next;
	t_map				*map;
}				t_spheres;

typedef struct s_planes
{
	float			x;
	float			y;
	float			z;
	float			nx;
	float			ny;
	float			nz;
	int				r;
	int				g;
	int				b;
	struct s_planes	*next;
	t_map			*map;
}				t_planes;

typedef struct s_cylinders
{
	float				x;
	float				y;
	float				z;
	float				nx;
	float				ny;
	float				nz;
	float				diameter;
	float				height;
	int					r;
	int					g;
	int					b;
	struct s_cylinders	*next;
	t_map				*map;
}				t_cylinders;

typedef struct s_element_count
{
	int		ambient;
	int		camera;
	int		light;
	int		sphere;
	int		plane;
	int		cylinder;
}				t_element_count;

typedef struct s_map
{
	t_ambient		*ambient;
	t_camera		*camera;
	t_lights		*lights;
	t_spheres		*spheres;
	t_planes		*planes;
	t_cylinders		*cylinders;
	t_element_count	*element_count;
}				t_map;

/* ************************************************************************** */
/*                                 parsing_utils.c                            */
/* ************************************************************************** */

int		validate_lines(char *line, t_element_count *element_count, t_map *map);
int		check_element_count(t_element_count *element_count, int flag);
int		free_split(char **split);
int		pos_decimal_check(char *str);
int		rgb_check(char *rgb, int min, int max);
double	ft_atof(const char *str);
int		vectors_check(char *str);
int		degree_check(char *str, int min, int max);
int		xyz_check(char *str);
int		decimal_check(char *str, int min, int max);
int		read_to_parse(t_element_count *element_count, t_map *map, char **file);

/* ************************************************************************** */
/*                                 validate_camera                            */
/* ************************************************************************** */

int		decimal_check(char *str, int min, int max);
int		validate_camera(char *line, t_element_count *element_count, t_map *map);

/* ************************************************************************** */
/*                                 validate shapes                            */
/* ************************************************************************** */

int		validate_sphere(char *line, t_element_count *element_count, t_map *map);
int		validate_plane(char *line, t_element_count *element_count, t_map *map);
int		validate_cylinder(char *line, t_element_count *element_count, \
		t_map *map);

/* ************************************************************************** */
/*                                 validate lights                            */
/* ************************************************************************** */

int		validate_light(char *line, t_element_count *element_count, t_map *map);
int		validate_ambient(char *line, t_element_count *element_count, \
		t_map *map);

/* ************************************************************************** */
/*                                 	data setup                                */
/* ************************************************************************** */

int		setup_data(t_element_count *element_count, t_map *map);
int		setup_ambient(char **split, t_map *map);
int		setup_camera(char **split, t_map *map);
int		setup_light(char **split, t_map *map);
int		setup_spheres(char **split, t_map *map);
int		setup_plane(char **split, t_map *map);
int		setup_cylinder(char **split, t_map *map);
int		terminate_map_data(t_map *map, char *error);
void	terminate_var_data(t_var *var, char *error);

/* ************************************************************************** */
/*                                 	util functions                            */
/* ************************************************************************** */

int		print_data(t_map *map);
void	replace_tabs_with_spaces(char *line);

#endif