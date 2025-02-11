NAME = pipex
NAME_BONUS = pipex_bonus
CC = cc
FLAGS = -Wall -Werror -Wextra -g #-fsanitize=address 
RM = rm -rf
LIBFT = library/libft/libft.a
LIBFT_FLAGS = -L./library/libft -lft

SRCS	=	pipex.c free.c list.c here_doc.c parsing.c

SRCS_BONUS	=	pipex_bonus.c\
				free_bonus.c\
				list_bonus.c\
				here_doc_bonus.c\
				parsing_bonus.c\

OBJS_BONUS = $(SRCS_BONUS:.c=.o)
OBJS = $(SRCS:.c=.o)

TARGET = $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(MLX_FLAGS) $(LIBFT_FLAGS)

$(NAME_BONUS): $(LIBFT) $(OBJS_BONUS)
	@$(CC) $(FLAGS) -o $(NAME_BONUS) $(OBJS_BONUS) $(MLX_FLAGS) $(LIBFT_FLAGS)

$(LIBFT):
	make -C library/libft

bonus : $(NAME_BONUS)

.c.o:
	$(CC) $(FLAGS) -c -o $@ $<

all: $(NAME)

clean:
	@$(RM) $(OBJS) $(OBJS_BONUS)
	@make -s -C library/libft clean
	@echo "files erased successfully"

fclean: clean
	@$(RM) $(NAME) $(NAME_BONUS)
	@make -s -C library/libft fclean

re: fclean all

bonus_re: fclean bonus

.PHONY: all clean fclean re bonus bonus_re
