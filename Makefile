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
#vpath %.c ./source
#vpath %.h ./source

vpath %.h ./source
vpath %.c ./source
SRCDIR=./source
CC=gcc
INCLUDE=C:\msys64\mingw64\include\SDL2

CFLAGS=-g -I$(INCLUDE) -c -Wall -O

LDFLAGS = -lSDL2main -lSDL2_image -lSDL2 -lm -lSDL2_net
#LDFLAGS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

# Define paths to object files based on the source files

# Define the final executable file
EXECUTABLE=simpleSDLexample1

# Default rule
all: $(EXECUTABLE)

# Linking all the object files to the executable

$(EXECUTABLE): $(SRCDIR)/main.o $(SRCDIR)/GameModel.o $(SRCDIR)/GameView.o $(SRCDIR)/GameController.o
	$(CC) $(SRCDIR)/main.o $(SRCDIR)/GameModel.o  $(SRCDIR)/GameView.o $ $(SRCDIR)/GameController.o -o $(EXECUTABLE) $(LDFLAGS)

# Compiling the main file into its object file
$(SRCDIR)/main.o: main.c $(SRCDIR)/GameModel.h GameView.h GameController.h
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $(SRCDIR)/main.o $(LDFLAGS)

# Compiling the model file into its object file
$(SRCDIR)/GameModel.o: GameModel.c GameModel.h GameView.h GameController.h
	$(CC) $(CFLAGS) $(SRCDIR)/GameModel.c -o $(SRCDIR)/GameModel.o $(LDFLAGS)

# Compiling the view file into its object file
$(SRCDIR)/GameView.o: GameView.c GameView.h GameController.h
	$(CC) $(CFLAGS) $(SRCDIR)/GameView.c -o $(SRCDIR)/GameView.o $(LDFLAGS)

# Compiling the controller file into its object file
$(SRCDIR)/GameController.o: GameController.c GameController.h
	$(CC) $(CFLAGS) $(SRCDIR)/GameController.c -o $(SRCDIR)/GameController.o $(LDFLAGS)

# Clean up command
clean:
	@echo "cleaning..."
	rm -rf $(SRCDIR)/*.o
	rm -f $(EXECUTABLE)
	@echo "Success"
