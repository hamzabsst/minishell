CFLAGS = -Wall -Wextra -Werror
CC = cc

NAME = myLib.a

FILES = $(wildcard *.c)

SRCS = $(FILES)

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
		@echo "Creating archive: ${NAME}"
		@ar rc ${NAME} ${OBJS}
		@echo "Compilation finished successfully!"

%.o: %.c libft.h
		@echo "Compiling $<"
		@${CC} ${CFLAGS} -c $< -o $@

clean:
		@echo "Cleaning mylib object files"
		@rm -f ${OBJS}

fclean: clean
		@echo "Removing ${NAME}"
		@rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
