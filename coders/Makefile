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
	set_variable.c		\
	timing.c			\
	utils_mutex_lock.c	\
	utils_mutex.c		\
	utils.c				\

NAME= codexion

TSAN_FLAGS = -fsanitize=thread -g -O1
TSAN_NAME = $(NAME)_tsan

OBJDIR= .obj
TSAN_OBJDIR= .obj_tsan

OBJS= $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS= $(OBJS:.o=.d)

TSAN_OBJS= $(SRCS:%.c=$(TSAN_OBJDIR)/%.o)
TSAN_DEPS= $(TSAN_OBJS:.o=.d)


all: $(NAME)

$(NAME): $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

val: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME) 4 10000 100 100 100 1 100 "edf"

tsan: $(TSAN_NAME)

$(TSAN_OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(TSAN_FLAGS) -c $< -o $@

$(TSAN_NAME): $(TSAN_OBJS)
	$(CC) $(CFLAGS) $(TSAN_FLAGS) $(TSAN_OBJS) -o $(TSAN_NAME)

run_tsan: tsan
	setarch $$(uname -m) -R ./$(TSAN_NAME) 8 1000 100 100 100 3 10 "edf"

helgrind: all
	valgrind --tool=helgrind --history-level=full ./$(NAME) 8 1000 100 100 100 3 10 "edf"

clean:
	rm -rf $(OBJS) $(DEPS) $(OBJDIR) $(TSAN_OBJS) $(TSAN_DEPS) $(TSAN_OBJDIR)

fclean: clean
	rm -f $(NAME) $(TSAN_NAME)

re: fclean all


-include $(DEPS) $(TSAN_DEPS) $(DEPS_bonus)

.PHONY: all clean fclean re
