CC = clang

EXECUTABLE_NAME := des_tools

SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

CXXFLAGS := -Werror -Wextra -Wall -Wno-unused -Wno-unused-parameter -O3

build/$(EXECUTABLE_NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<


run:
	./build/$(EXECUTABLE_NAME)

do: ./build/$(EXECUTABLE_NAME) run

clean:
	@rm -rf obj/*.o
	@rm -rf build/*