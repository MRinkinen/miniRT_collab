/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:26 by mrinkine          #+#    #+#             */
/*   Updated: 2024/10/26 03:08:26 by tvalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/parsing.h"

int	init_scene(t_var *var, t_map *map)
{
	if (mlxinit(var) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_ambient_color(var, map) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (initialize_camera(var, &var->cam, map) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_objects(var, map) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_light(var, map) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (var->objects == NULL == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int parse_file(t_element_count *element_count, t_map **map, char **argv)
{
    *map = NULL;
    ft_memset(element_count, 0, sizeof(t_element_count));
    *map = malloc(sizeof(t_map));
    if (!*map)
        return (EXIT_FAILURE);
    ft_bzero(*map, sizeof(t_map));
    setup_data(element_count, *map);
    if (read_to_parse(element_count, *map, argv) == 0)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    t_var var;
    t_element_count element_count;
    t_map *map;

    if (argc != 2)
    {
        ft_printf("Error in arguments, just give a map filename\n");
        return (0);
    }
    if (parse_file(&element_count, &map, argv) == EXIT_FAILURE)
    {
        terminate_map_data(map, &var, "Error while parsing data\n");
        return (EXIT_FAILURE);
    }
    if (init_scene(&var, map) == EXIT_FAILURE)
    {
        terminate_map_data(map, &var, "Error in init scene\n");
        return (EXIT_FAILURE);
    }
    printimage(&var);
    hooks(&var);
    mlx_loop(var.mlx);
    terminate_var_data(&var, "program ended successfully\n");
    terminate_map_data(map, &var, "program ended successfully\n");
    return (EXIT_SUCCESS);
}
