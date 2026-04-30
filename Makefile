NAME		:= codexion
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror  -pthread -g3 -fsanitize=leak

SRCDIR		:= .
DEPDIR		:= .deps
INCDIR		:= includes
BUILDDIR	:= build

SRCFILES	:= main.c utils/ft_calloc.c utils/print_instructions.c utils/init_dongles.c \
			   utils/get_time.c compile.c utils/print_lock.c monitor.c utils/check_exit.c \
			   queue/queue.c utils/mutexes.c queue/edf.c utils/init_coders.c utils/init_config.c \
			   debug.c refactor.c

OBJS		:= $(addprefix $(BUILDDIR)/,$(SRCFILES:.c=.o))
HEADERS		:= $(INCDIR)/coders.h


DEPFLAGS	:= -MD -MP -MF $(DEPDIR)/$*.d


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


$(BUILDDIR)/%.o: %.c $(HEADERS)
	mkdir -p $(dir $@) $(DEPDIR)/$(dir $<)
	$(CC) $(CFLAGS) $(DEPFLAGS) -I$(INCDIR) -c $< -o $@

vpath %.c $(SRCDIR)
-include $(addprefix $(DEPDIR)/,$(SRCFILES:.c=.d))

clean:
	rm -rf $(DEPDIR)
	rm -rf $(BUILDDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: all
	$(CC) $(CFLAGS) -g3 $(OBJS) -o $(NAME)
	valgrind --leak-check=full -s ./$(NAME) 7 3000 40 150 100 3 10 fifo
# --trace-children=yes --track-fds=yes --tool=helgrind -s
gdb_debug: re $(OBJS)
	$(CC) $(CFLAGS) -g3 $(OBJS) -o $(NAME)
	gdb --args ./$(NAME) $(ARG) 3 100 200 150 100 5 1 str


.PHONY: all clean fclean re debug gdb_debug

# valgrind --tool=helgrind -s ./$(NAME)
# valgrind --leak-check=full --show-leak-kinds=all -s