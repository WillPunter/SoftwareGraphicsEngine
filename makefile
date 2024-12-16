# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -g

# Output executable
OUTPUT = ./build/example_1

# Source
SRC = ./src/examples/hello_world/main.c
SRC += ./src/system/window_x11.c
SRC += ./src/graphics/renderer.c

# Libraries to Link
LIBS = -lX11 -lm

# Build the executable
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT) $(LIBS)

# Clean
clean:
	rm -f $(OUTPUT)

# Run the program
run: $(OUTPUT)
	./$(OUTPUT)