NAME		:= codexion
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror  -pthread

SRCDIR		:= .
DEPDIR		:= .deps
INCDIR		:= includes
BUILDDIR	:= build

SRCFILES	:= main.c utils/ft_calloc.c utils/print_instructions.c utils/init_structs.c \
			   utils/get_time.c utils/actions.c

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
	valgrind --trace-children=yes --track-fds=yes --tool=helgrind -s ./$(NAME) 8 500 300 200 300 5 7 str

gdb_debug: re $(OBJS)
	$(CC) $(CFLAGS) -g3 $(OBJS) -o $(NAME)
	gdb --args ./$(NAME) $(ARG) 8 500 300 200 300 5 7 str


.PHONY: all clean fclean re debug gdb_debug

# valgrind --tool=helgrind -s ./$(NAME)
# valgrind --leak-check=full --show-leak-kinds=all -s