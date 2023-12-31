# Compiler settings
CC = gcc
CFLAGS = -std=c11 -Wall -I./include -g

# Source files
SRCS = src/main.c 

# Build directory for object files (x86)
BUILD_DIR = ./build/x86

# Object files (placed in the build directory)
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

# Output executable
EXEC = $(BUILD_DIR)/pico_yatrt_x86

# Build rule
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) -lm

# Object files rule
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

# Phony targets
.PHONY: all clean
