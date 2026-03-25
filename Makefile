NAME		:= codexion
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -pthread

SRCDIR		:= .
DEPDIR		:= .deps
INCDIR		:= includes
BUILDDIR	:= build

SRCFILES	:= main.c init_coders.c utils/ft_calloc.c

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
	valgrind --leak-check=full ./$(NAME)

gdb_debug: re $(OBJS)
	$(CC) $(CFLAGS) -g3 $(OBJS) -o $(NAME)
	gdb --args ./$(NAME) $(ARG)


.PHONY: all clean fclean re force debug gdb_debug