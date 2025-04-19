NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

MYLIB_DIR	= mylib
MYLIB		= $(MYLIB_DIR)/myLib.a

<<<<<<< HEAD
SRCS	=	src/abdo/main.c \
=======
SRCS	=	src/hamza/main.c \
>>>>>>> 79d8ee84f90883a8b384d8957bc4b2eb8dbabe87

OBJS	= $(SRCS:.c=.o)

all: $(MYLIB) $(NAME)
	@echo "build complete \033[1;34m✔\033[0m"

$(MYLIB):
	@$(MAKE) -s -C $(MYLIB_DIR)

%.o: %.c inc/minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(MYLIB)
	@$(CC) $(CFLAGS) $(OBJS) $(MYLIB) -o $(NAME) -lreadline -lhistory

clean:
	@rm -f $(OBJS)
	@$(MAKE) -s clean -C $(MYLIB_DIR)
	@echo "clean completed \033[1;34m✔\033[0m"


fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -s fclean -C $(MYLIB_DIR)
	@echo "full clean completed \033[1;31m✔\033[0m"

re: fclean all

.PHONY: all clean fclean re
