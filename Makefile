vpath %.c src src_bonus
NAME		= pipex
BONUS		= pipex
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
# CFLAGS		= -ggdb
AR			= ar rcs
INCLUDES	= -I./headers/ -I./libft/headers/
LIBFT_DIR	= libft
LIBFT		= $(addprefix $(LIBFT_DIR)/, libft.a)
SRC_DIR		= src
BONUS_DIR	= src_bonus
SRC			= pipex.c \
			  main.c \
			  utils.c
SRC_BONUS	= pipex_bonus.c \
			  main_bonus.c \
			  utils_bonus.c \
			  get_next_line.c \
			  get_next_line_utils.c
OBJ_DIR		= obj
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
OBJ_BONUS	= $(addprefix $(OBJ_DIR)/, $(SRC_BONUS:.c=.o))


all: $(NAME)

bonus:	$(BONUS)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) -o $(NAME)

$(BONUS): $(LIBFT) $(OBJ_BONUS)
	$(CC) $(OBJ_BONUS) $(LIBFT) -o $(BONUS)

$(OBJ_DIR)/%.o: %.c
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
