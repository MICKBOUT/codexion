CC= cc
CFLAGS= -Werror -Wextra -Wall -MMD -MP -Iincludes

SRCDIR= src
SRCS=\
	main.c

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
	valgrind ./$(MAIN) 8 7 6 5 4 3 2 "edf"

clean:
	rm -rf $(OBJS) $(DEPS) $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS) $(DEPS_bonus)

.PHONY: all clean fclean re