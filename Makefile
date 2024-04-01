# Makefile for Windows
# SRCDIR=./source
# CC=gcc
# INCLUDE = C:\msys64\mingw64\include\SDL2

# CFLAGS = -g -I$(INCLUDE) -c 
# LDFLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

# simpleSDLexample1: main.o
# 	$(CC) main.o -o simpleSDLexample1 $(LDFLAGS)

# main.o: $(SRCDIR)/main.c
# 	$(CC) $(CFLAGS) $(SRCDIR)/main.c

# clean:
# 	rm *.exe
# 	rm *.o

# Makefile for Windows
SRCDIR=./source
CC=gcc
INCLUDE=C:\msys64\mingw64\include\SDL2

CFLAGS=-g -I$(INCLUDE) -I$(SRCDIR)/model -I$(SRCDIR)/view -I$(SRCDIR)/controller
LDFLAGS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

# Define paths to object files based on the source files
MAIN_O=$(SRCDIR)/main.o
MODEL_O=$(SRCDIR)/model/GameModel.o
VIEW_O=$(SRCDIR)/view/GameView.o
CONTROLLER_O=$(SRCDIR)/controller/GameController.o

# Define the final executable file
EXECUTABLE=simpleSDLexample1

# Default rule
all: $(EXECUTABLE)

# Linking all the object files to the executable
$(EXECUTABLE): $(MAIN_O) $(MODEL_O) $(VIEW_O) $(CONTROLLER_O)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compiling the main file into its object file
$(MAIN_O): $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $< -o $@

# Compiling the model file into its object file
$(MODEL_O): $(SRCDIR)/model/GameModel.c $(SRCDIR)/model/GameModel.h
	$(CC) $(CFLAGS) $< -o $@

# Compiling the view file into its object file
$(VIEW_O): $(SRCDIR)/view/GameView.c $(SRCDIR)/view/GameView.h
	$(CC) $(CFLAGS) $< -o $@

# Compiling the controller file into its object file
$(CONTROLLER_O): $(SRCDIR)/controller/GameController.c $(SRCDIR)/controller/GameController.h
	$(CC) $(CFLAGS) $< -o $@

# Clean up command
clean:
	del /Q $(MAIN_O) $(MODEL_O) $(VIEW_O) $(CONTROLLER_O) $(EXECUTABLE).exe

