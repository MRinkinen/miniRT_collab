# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrinkine <mrinkine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/27 16:28:29 by tvalimak          #+#    #+#              #
#    Updated: 2024/10/28 14:27:04 by mrinkine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
BONUS_NAME = miniRT_bonus
OBJ_DIR = ./obj
OBJ_BONUS_DIR = ./objbonus
INCLUDEDIR = ./includes/

SRC_DIR = ./sources
SRC_BON_DIR = ./sources_bonus

SOURCES = $(addprefix $(SRC_DIR)/, \
minirt.c functions.c functions_two.c functions_three.c functions_four.c functions_five.c functions_six.c functions_seven.c\
ray.c color.c mlxfunctions.c mlxfunctions_two.c parsing_utils_two.c parsing_utils.c parsing_utils_three.c setup_ambient.c setup_camera.c \
setup_cylinder.c setup_light.c setup_plane.c setup_sphere.c terminate.c terminate_two.c \
validate_ambient.c validate_camera.c validate_cylinder.c validate_light.c validate_plane.c validate_sphere.c \
plane.c cylinder.c cylinder_two.c intersect_cylinder.c sphere.c light.c init.c parse.c render.c renderhelper.c camera.c tuple.c math.c)

SOURCES_BONUS = $(addprefix $(SRC_BON_DIR)/, \
minirt_bonus.c functions_bonus.c functions_two_bonus.c functions_three_bonus.c functions_four_bonus.c functions_five_bonus.c functions_six_bonus.c functions_seven_bonus.c\
ray_bonus.c color_bonus.c mlxfunctions_bonus.c mlxfunctions_two_bonus.c parsing_utils_two_bonus.c parsing_utils_bonus.c parsing_utils_three_bonus.c setup_ambient_bonus.c setup_camera_bonus.c \
setup_cylinder_bonus.c setup_light_bonus.c setup_plane_bonus.c setup_sphere_bonus.c terminate_bonus.c terminate_two_bonus.c render_low_bonus.c\
validate_ambient_bonus.c validate_camera_bonus.c validate_cylinder_bonus.c validate_light_bonus.c validate_plane_bonus.c validate_sphere_bonus.c \
plane_bonus.c cylinder_bonus.c cylinder_two_bonus.c intersect_cylinder_bonus.c sphere_bonus.c light_bonus.c init_bonus.c parse_bonus.c render_bonus.c renderhelper_bonus.c camera_bonus.c tuple_bonus.c math_bonus.c)

OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.c=.o)))
BONUS_OBJECTS = $(addprefix $(OBJ_BONUS_DIR)/, $(notdir $(SOURCES_BONUS:.c=.o)))

USERNAME = $(USER)
LIBFTNAME = libft.a
LIBFTDIR = ./libft/
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = $(LIBFTDIR)$(LIBFTNAME)
MLX = MLX42/build/libmlx42.a

MLX_HEADER = MLX42/include/MLX42/MLX42.h

target asan: CFLAGS += -fsanitize=address,undefined -g
export CFLAGS

all: $(NAME)

bonus: $(BONUS_NAME)

$(LIBFT) :
	make -C ./libft

$(MLX) :
	cd MLX42 && cmake -B build && cmake --build build -j4

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_BONUS_DIR):
	mkdir -p $(OBJ_BONUS_DIR)

# Link the executable
$(NAME) : $(OBJ_DIR) $(OBJECTS) $(MLX) $(LIBFT)
	@rm -f $(BONUS_OBJECTS)
	@rm -rf $(OBJ_BONUS_DIR)
	@rm -rf $(BONUS_NAME)
	$(CC) $(CFLAGS) $(OBJECTS) $(MLX) $(LIBFT) -ldl -pthread -lm -L"/Users/$(USERNAME)/.brew/opt/glfw/lib/" -lglfw -I $(MLX_HEADER) -o $(NAME)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(BONUS_NAME): $(OBJ_BONUS_DIR) $(BONUS_OBJECTS) $(MLX) $(LIBFT)
	@rm -f $(OBJECTS)
	@rm -rf $(OBJ_DIR)
	@rm -rf $(NAME)
	$(CC) $(CFLAGS) $(BONUS_OBJECTS) $(MLX) $(LIBFT) -ldl -pthread -lm -L"/Users/$(USERNAME)/.brew/opt/glfw/lib/" -lglfw -I $(MLX_HEADER) -o $(BONUS_NAME)


$(OBJ_BONUS_DIR)/%.o: $(SRC_BON_DIR)/%.c | $(OBJ_BONUS_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(BONUS_OBJECTS)
	rm -rf $(OBJ_DIR)
	rm -rf $(OBJ_BONUS_DIR)
	rm -rf MLX42/build
	rm -f .bonus
	make fclean -C ./libft

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus clean_name clean_name_bonus
