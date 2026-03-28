NAME := woody_woodpacker
TEST_NAME := woddy_tests

CC := cc
CFLAGS := -Wall -Wextra -Werror

SRC_DIR := src
INC_DIR := include
BUILD_DIR := .build
TEST_DIR := tests

CPPFLAGS := -I$(INC_DIR) -MMD -MP

SRCS := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_SRC := $(TEST_DIR)/tests.c
TEST_OBJ := $(BUILD_DIR)/$(TEST_DIR)/tests.o

DEPS 	:= $(OBJS:.o=.d) $(TEST_OBJ:.o=.d)

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TEST_NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: $(OBJS) $(TEST_OBJ)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(TEST_OBJ) $(filter-out $(BUILD_DIR)/main.o, $(OBJS)) -o $(TEST_NAME)
	@./$(TEST_NAME)
	@rm -f $(TEST_NAME)

-include $(DEPS)
