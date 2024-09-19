NAME = miniRT
SOURCES = $(addprefix $(SRC_DIR)/, \
minirt.c vector3.c functions.c ray.c color.c mlxfunctions.c parse.c parsing_utils_2.c parsing_utils.c \
setup_ambient.c setup_camera.c setup_cylinder.c setup_light.c setup_plane.c setup_sphere.c structs_n_lists.c \
validate_ambient.c validate_camera.c validate_cylinder.c validate_light.c validate_plane.c validate_sphere.c)
#SOURCES_BONUS = $(addprefix $(SRC_BON_DIR)/, \
#sz_long_bonus.c checkfunctions_bonus.c enemy_bonus.c enemymove_bonus.c vortex_bonus.c input_bonus.c moveplayer_bonus.c world_functions_bonus.c help_functions_bonus.c player_functions_bonus.c wall_functions_bonus.c collectable_functions_bonus.c ground_functions_bonus.c levelend_functions_bonus.c)

OBJECTS = $(SOURCES:.c=.o)
#BONUS_OBJECTS = $(SOURCES_BONUS:.c=.o)

SRC_DIR	= ./sources
#SRC_BON_DIR = ./sources_bonus
USERNAME = mattirinkinen
LIBFTNAME = libft.a
LIBFTDIR = ./libft/
INCLUDEDIR = ./includes/
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = $(LIBFTDIR)$(LIBFTNAME)
MLX = MLX42/build/libmlx42.a

MLX_HEADER = MLX42/include/MLX42/MLX42.h

all: $(NAME)

$(LIBFT) :
	make -C ./libft

$(MLX) :
	cd MLX42 &&	cmake -B build && cmake --build build -j4

$(NAME) : $(OBJECTS) $(MLX) $(LIBFT)
	$(CC) $(OBJECTS) $(MLX) $(LIBFT) -ldl -pthread -lm -L"/Users/$(USERNAME)/.brew/opt/glfw/lib/" -lglfw -I $(MLX_HEADER) -o $(NAME)

#bonus: .bonus

#.bonus : $(BONUS_OBJECTS) $(MLX) $(LIBFT)
#	cc $(BONUS_OBJECTS) $(MLX) $(LIBFT) -ldl -pthread -lm -L"/Users/$(USERNAME)/.brew/opt/glfw/lib/" -lglfw -I $(MLX_HEADER) -o $(NAME)
#	@touch .bonus

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
#$(BONUS_OBJECTS)
	rm -rf MLX42/build
#	@rm -f .bonus
	make fclean -C ./libft

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
