CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Iincludes
NAME = ft_ping

SRCDIR = srcs
OBJDIR = objs
INCDIR = includes

SOURCES = main.c parser.c
SRCS = $(addprefix $(SRCDIR)/, $(SOURCES))
OBJS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean 
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
