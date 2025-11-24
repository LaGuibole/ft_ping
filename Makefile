CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Iincludes -lm
NAME = ft_ping

SRCDIR = srcs
OBJDIR = objs
INCDIR = includes

SOURCES = main.c parser.c resolve_addr.c socket_icmp.c icmp_packets.c send_packets.c receive.c stats.c utils.c
SRCS = $(addprefix $(SRCDIR)/, $(SOURCES))
OBJS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

all: $(NAME)

LDLIBS = -lm

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

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
