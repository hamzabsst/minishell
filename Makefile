NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Iinc

MYLIB_DIR	= mylib
MYLIB		= $(MYLIB_DIR)/myLib.a

SRCS	=	src/parsing/main.c \
			src/parsing/token.c \
 			src/parsing/utils.c \
 			src/parsing/parsing.c \

# src/main.c
# src/builtin/cd.c
# src/builtin/builtin.c
# src/builtin/export.c
# src/builtin/unset.c
# src/builtin/exit.c
# src/pipe/pipe.c
# src/pipe/get_cmds.c
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
