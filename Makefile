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

#makefile for windows

# SRCDIR=./source
# CC=gcc
# INCLUDE=C:\msys64\mingw64\include\SDL2

# CFLAGS = -g -I$(INCLUDE) -c
# LDFLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

# GSwitch: main.o model.o view.o controller.o 



# #Makefile for Windows
# SRCDIR=./source
# CC=gcc
# INCLUDE=C:\msys64\mingw64\include\SDL2

# CFLAGS=-g -I$(INCLUDE) -I$(SRCDIR)/model -I$(SRCDIR)/view -I$(SRCDIR)/controller
# LDFLAGS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

# # Define paths to object files based on the source files
# MAIN_O=$(SRCDIR)/main.o
# MODEL_O=$(SRCDIR)/model/GameModel.o
# VIEW_O=$(SRCDIR)/view/GameView.o
# CONTROLLER_O=$(SRCDIR)/controller/GameController.o

# # Define the final executable file
# EXECUTABLE=GSwitch

# # Default rule
# all: $(EXECUTABLE)

# # Linking all the object files to the executable
# $(EXECUTABLE): $(MAIN_O) $(MODEL_O) $(VIEW_O) $(CONTROLLER_O)
# 	@echo Linking: $(CC) $^ -o $@ $(LDFLAGS)
# 	$(CC) $^ -o $@ $(LDFLAGS)

# # Compiling the main file into its object file
# $(MAIN_O): $(SRCDIR)/main.c
# 	$(CC) $(CFLAGS) $< -o $@

# # Rules for compiling the .c files into .o files
# $(MODEL_O): $(SRCDIR)/model/GameModel.c $(SRCDIR)/model/GameModel.h
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(VIEW_O): $(SRCDIR)/view/GameView.c $(SRCDIR)/view/GameView.h
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(CONTROLLER_O): $(SRCDIR)/controller/GameController.c $(SRCDIR)/controller/GameController.h
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Clean up command
# clean:
# 	del /Q $(MAIN_O) $(MODEL_O) $(VIEW_O) $(CONTROLLER_O) $(EXECUTABLE).exe




# Makefile for Windows
SRCDIR=./source
CC=gcc
INCLUDE=C:\msys64\mingw64\include\SDL2

CFLAGS=-g -I$(INCLUDE)
LDFLAGS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

# Define object files
OBJS=$(SRCDIR)/main.o $(SRCDIR)/GameModel.o $(SRCDIR)/GameView.o $(SRCDIR)/GameController.o

# Define the final executable file
EXECUTABLE=GSwitch

# Default rule
all: $(EXECUTABLE)

# Linking all the object files to the executable
$(EXECUTABLE): $(OBJS)
	@echo Linking: $(CC) $^ -o $@ $(LDFLAGS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Pattern rule for compiling the .c files into .o files
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up command
clean:
	rm *.exe
	rm *.o