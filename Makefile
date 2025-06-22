NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Iinc -g


MYLIB_DIR	= mylib
MYLIB		= $(MYLIB_DIR)/myLib.a

# preload_test:
# 	LD_PRELOAD=./fail.so ./$(NAME)

SRCS := $(shell find src -name '*.c')
#!need to modify this later

OBJS	= $(SRCS:.c=.o)

all: $(MYLIB) $(NAME)
	@echo "build complete \033[1;34m✔\033[0m"

$(MYLIB):
	@$(MAKE) -s -C $(MYLIB_DIR)

%.o: %.c inc/minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(MYLIB)
	@$(CC) $(CFLAGS) $(OBJS) $(MYLIB) -o $(NAME) -lreadline -lhistory

#!for gcc
# $(NAME): $(OBJS) $(MYLIB)
# 	@$(CC) $(CFLAGS) $(OBJS) $(MYLIB) -o $(NAME) -lreadline -lhistory -no-pie

clean:
	@rm -f $(OBJS)
	@$(MAKE) -s clean -C $(MYLIB_DIR)
	@echo "clean completed \033[1;34m✔\033[0m"


fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -s fclean -C $(MYLIB_DIR)
	@echo "full clean completed \033[1;31m✔\033[0m"

t: clean all
	@rm -f $(OBJS)
	@./$(NAME)

re: fclean all

.PHONY: all clean fclean re
