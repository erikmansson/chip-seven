NAME = chip-seven
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
OBJ_DIR = obj

SOURCE_DIRS = $(SRC_DIR) $(LIB_DIR)
SOURCES = $(foreach d,$(SOURCE_DIRS),$(shell find $(d) -name '*.c'))
OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))

CC = gcc
INCLUDES = -I$(INCLUDE_DIR) -I$(LIB_DIR)
CFLAGS = -c -W -Wall -Og -g $(INCLUDES)
LDFLAGS = `pkg-config --static --libs glfw3`

.PHONY: all clean

$(OBJ_DIR)/%.o: %.c
	@ mkdir -p $(OBJ_DIR)/$(dir $<)
	@ $(CC) $(CFLAGS) $< -o $@

all: $(OBJECTS)
	@ $(CC) $(OBJECTS) $(LDFLAGS) -o $(NAME)

clean:
	@ rm -rf $(OBJ_DIR)
	@ rm -f $(NAME)
