NAME := woody_woodpacker

CC := cc
CFLAGS := -Wall -Wextra -Werror

SRC_DIR := src
INC_DIR := include
BUILD_DIR := .build

CPPFLAGS := -I$(INC_DIR) -MMD -MP

SRCS := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

DOCKER_CONTAINER_DEV := woody-dev
DOCKER_PLATFORM := linux/amd64

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

docker-build:
	docker build --platform=$(DOCKER_PLATFORM) -t $(NAME) .

docker-dev: docker-build
	docker run -it --rm \
		--platform=$(DOCKER_PLATFORM) \
		--name $(DOCKER_CONTAINER_DEV) \
		--cap-add=NET_RAW \
		-v $(PWD):/app \
		-w /app \
		$(NAME) bash

-include $(DEPS)
