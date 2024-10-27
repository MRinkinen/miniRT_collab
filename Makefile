# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tvalimak <tvalimak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/27 16:28:29 by tvalimak          #+#    #+#              #
#    Updated: 2024/10/27 16:30:02 by tvalimak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
SRC_DIR = ./sources
OBJ_DIR = ./obj
INCLUDEDIR = ./includes/

SOURCES = $(addprefix $(SRC_DIR)/, \
minirt.c functions.c functions_two.c functions_three.c functions_four.c functions_five.c functions_six.c functions_seven.c\
ray.c color.c mlxfunctions.c mlxfunctions_two.c parsing_utils_two.c parsing_utils.c parsing_utils_three.c setup_ambient.c setup_camera.c \
setup_cylinder.c setup_light.c setup_plane.c setup_sphere.c terminate.c terminate_two.c \
validate_ambient.c validate_camera.c validate_cylinder.c validate_light.c validate_plane.c validate_sphere.c \
plane.c cylinder.c cylinder_two.c intersect_cylinder.c sphere.c light.c init.c parse.c render.c renderhelper.c camera.c tuple.c math.c)

OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.c=.o)))

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

$(LIBFT) :
	make -C ./libft

$(MLX) :
	cd MLX42 && cmake -B build && cmake --build build -j4

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link the executable
$(NAME) : $(OBJ_DIR) $(OBJECTS) $(MLX) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJECTS) $(MLX) $(LIBFT) -ldl -pthread -lm -L"/Users/$(USERNAME)/.brew/opt/glfw/lib/" -lglfw -I $(MLX_HEADER) -o $(NAME)

# Compile each .c file into an object file in the obj directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -rf $(OBJ_DIR)
	rm -rf MLX42/build
	make fclean -C ./libft

fclean: clean
	rm -f $(NAME)

re: fclean all
asan: fclean all

.PHONY: all clean fclean re asan
