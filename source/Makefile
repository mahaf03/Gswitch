# # CC = gcc
# # CFLAGS = -Wall -g
# # LDFLAGS = 
# # OBJFILES = main.o GameController.o GameModel.o GameView.o
# # TARGET = GameExecutable

# # all: $(TARGET)

# # $(TARGET): $(OBJFILES)
# # 	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

# # main.o: main.c GameModel.h GameController.h GameView.h
# # 	$(CC) $(CFLAGS) -c main.c

# # GameController.o: GameController.c GameController.h GameModel.h GameView.h
# # 	$(CC) $(CFLAGS) -c GameController.c

# # GameModel.o: GameModel.c GameModel.h
# # 	$(CC) $(CFLAGS) -c GameModel.c

# # GameView.o: GameView.c GameView.h
# # 	$(CC) $(CFLAGS) -c GameView.c

# # clean:
# # 	rm -f $(OBJFILES) $(TARGET)

# # .PHONY: all clean
# CC=clang  # Compiler to use, clang is standard on macOS
# CFLAGS=-Wall -Wextra -g  # Compiler flags
# LDFLAGS=-lm  # Linker flags, e.g., link against the math library
# FRAMEWORKS=  # macOS frameworks, e.g., -framework OpenGL -framework Cocoa

# # Object files corresponding to source files
# OBJS=GameController.o GameModel.o GameView.o main.o

# # Target executable name
# TARGET=game_app

# # Rule to make everything (default rule)
# all: $(TARGET)

# # Rule to link the program
# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(FRAMEWORKS)

# # Rule to compile the source files
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $<

# # Rule to clean up
# clean:
# 	rm -f $(OBJS) $(TARGET)

# .PHONY: all clean
CC=/usr/bin/clang  # Compiler path as per JSON config
CFLAGS=-Wall -Wextra -g -I/opt/homebrew/include -I/opt/homebrew/include/SDL2  # Include SDL2 headers
LDFLAGS=-L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lm  # Link the SDL2 library, SDL2_image, SDL2_mixer, and math library
FRAMEWORKS=  # Assuming no specific frameworks needed beyond what SDL2 requires

# Object files
OBJS=GameController.o GameModel.o GameView.o main.o

# Target executable
TARGET=GSwitch

# Default rule
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(FRAMEWORKS)

# Compilation rule
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
