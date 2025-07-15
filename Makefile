NAME	= minishell
CC		= cc
CFLAGS	= -Iinc

MYLIB_DIR	= mylib
MYLIB		= $(MYLIB_DIR)/myLib.a

SRCS := src/execution/builtin/echo.c src/execution/builtin/cd.c src/execution/builtin/env.c \
		src/execution/builtin/exit.c src/execution/builtin/export.c src/execution/builtin/export_utils.c \
		src/execution/builtin/unset.c src/execution/execute/childs.c src/execution/execute/env_to_array.c \
		src/execution/execute/execute.c src/execution/execute/get_cmds.c src/execution/execute/parent.c \
		src/execution/ft_malloc.c src/execution/redirection/heredoc.c \
		src/execution/redirection/heredoc_utils.c src/execution/redirection/redirection.c \
		src/execution/utils.c src/interface.c src/main.c src/parsing/exit_code.c src/parsing/parsing.c \
		src/parsing/quotes/check_quotes.c src/parsing/quotes/syntax_error.c \
		src/parsing/split/count_token.c src/parsing/split/split.c src/parsing/split/split_utils.c \
		src/parsing/tokens/parse_cmds.c src/parsing/tokens/parse_redir.c src/parsing/tokens/tokenize.c \
		src/parsing/expansion.c \

OBJS	= $(SRCS:.c=.o)

all: $(MYLIB) $(NAME)

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
