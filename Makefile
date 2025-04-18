NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

MYLIB_DIR	= mylib
MYLIB		= $(MYLIB_DIR)/myLib.a

SRCS	=	src/main.c \

OBJS	= $(SRCS:.c=.o)

all: $(MYLIB) $(NAME)
	@echo "Build complete!"

$(MYLIB):
	@echo "Building myLib"
	@$(MAKE) -s -C $(MYLIB_DIR)

%.o: %.c inc/minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(MYLIB)
	@$(CC) $(CFLAGS) $(OBJS) $(MYLIB) -o $(NAME)

clean:
	@echo "Cleaning object files"
	@rm -f $(OBJS)
	@$(MAKE) -s clean -C $(MYLIB_DIR)

fclean: clean
	@echo "Removing excutable"
	@rm -f $(NAME)
	@$(MAKE) -s fclean -C $(MYLIB_DIR)

re: fclean all

.PHONY: all clean fclean re
