NAME		= pipex
CC			= cc
# CFLAGS		= -Wall -Wextra -Werror
CFLAGS		= -ggdb
AR			= ar rcs
INCLUDES	= -I./headers/ -I./libft/headers/
LIBFT_DIR	= libft
LIBFT		= $(addprefix $(LIBFT_DIR)/, libft.a)
SRC_DIR		= src
SRC			= pipex.c \
			  main.c \
			  utils.c
OBJ_DIR		= obj
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< $(INCLUDES) -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)/ all

clean:
	make -C $(LIBFT_DIR)/ clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT_DIR)/ fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re $(LIBFT)
