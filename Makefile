<<<<<<< HEAD
# ---------------------------------------------------------------------------- #
#                                    ft_ping                                   #
# ---------------------------------------------------------------------------- #

include Vars.mk

# -------------------------------- Compilation ------------------------------- #

$(NAME): $(OBJS)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)
	@echo "[$(TARGET_NAME)] Compiled successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# -------------------------------- Phony rules ------------------------------- #

.PHONY: all clean fclean re

all: $(NAME)

clean:
	@rm -f $(OBJS)
	@echo "[$(TARGET_NAME)] Object files removed!"

fclean: clean
	@rm -f $(NAME)
	@echo "[$(TARGET_NAME)] Executable removed!"

re: fclean all
	@echo "[$(TARGET_NAME)] Recompiled successfully!"

=======
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
>>>>>>> 2ccd1ee (+ | Init)
