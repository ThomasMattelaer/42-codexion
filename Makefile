NAME        := codexion

INCLUDE_DIR := include/
SRC_DIR     := src/
OBJ_DIR     := obj/
HEAP_DIR    := heap/
THREADS_DIR := threads/

SRC_FILES   := ${SRC_DIR}main.c \
               ${SRC_DIR}parsing.c \
			   ${SRC_DIR}${THREADS_DIR}time.c \
			   ${SRC_DIR}${THREADS_DIR}dongle.c \
			   ${SRC_DIR}${HEAP_DIR}action_heap.c \
			   ${SRC_DIR}${HEAP_DIR}create_heap.c \


OBJ := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES))

CFLAGS      := -Wall -Wextra -Werror -I $(INCLUDE_DIR) -g

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
