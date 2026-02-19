NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
THREADS = -pthread

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(SRC_DIR)/parse.c \
       $(SRC_DIR)/utils.c \
       $(SRC_DIR)/init.c \
       $(SRC_DIR)/dongle.c \
       $(SRC_DIR)/routine.c \
	   $(SRC_DIR)/printer.c
MAIN = codexion.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
MAIN_OBJ = $(OBJ_DIR)/codexion.o

all: $(NAME)

$(NAME): $(OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(OBJS) $(MAIN_OBJ) $(THREADS) -o $(NAME)

$(OBJ_DIR)/codexion.o: $(MAIN) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
