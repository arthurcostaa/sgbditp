NAME = sgbditp
SRC = $(NAME).c
BUILD_DIR = bin
FLAGS = -g -std=c99 -Wall -Wextra -Werror -pedantic

all: $(BUILD_DIR)/$(NAME)

$(BUILD_DIR)/$(NAME): $(SRC) | $(BUILD_DIR)
	gcc $(SRC) -o $(BUILD_DIR)/$(NAME) $(FLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean