NAME        := codexion

INCLUDE_DIR := include/
SRC_DIR     := src/
OBJ_DIR     := obj/

SRC_FILES   := ${SRC_DIR}main.c \
               ${SRC_DIR}parsing.c \
               ${SRC_DIR}threads.c \

OBJ := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES))

CFLAGS      := -Wall -Wextra -Werror -I $(INCLUDE_DIR)

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	cc $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
