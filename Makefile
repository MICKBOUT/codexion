CC= cc
CFLAGS= -Werror -Wextra -Wall -MMD -MP -Iincludes -g -pthread

SRCDIR= src
SRCS=\
	execute_coder.c		\
	exit.c				\
	init.c				\
	main.c				\
	parsing.c			\
	queue.c				\
	timing.c			\
	utils_mutex_lock.c	\
	utils_mutex.c		\
	utils.c				\

NAME= codexion

OBJDIR= .obj

OBJS= $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS= $(OBJS:.o=.d)


all: $(NAME)

$(NAME): $(OBJS) Makefile
	$(CC) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

val: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME) 4 10000 100 100 100 1 100 "edf"

clean:
	rm -rf $(OBJS) $(DEPS) $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS) $(DEPS_bonus)

.PHONY: all clean fclean re