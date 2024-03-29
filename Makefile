# # Makefile for Windows
# SRCDIR=./source
# CC=gcc
# INCLUDE = C:\msys64\mingw64\include\SDL2

# CFLAGS = -g -I$(INCLUDE) -c 
# LDFLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

# simpleSDLexample1: main.o
# 	$(CC) main.o -o simpleSDLexample1 $(LDFLAGS)

# main.o: $(SRCDIR)/main.c
# 	$(CC) $(CFLAGS) $(SRCDIR)/main.c

# simpleSDLexample2: model.o
# 	$(CC) model.o -o simpleSDLexample2 $(LDFLAGS)

# model.o: $(SRCDIR)/model.c
# 	$(CC) $(CFLAGS) $(SRCDIR)/model.c

# clean:
# 	rm *.exe
# 	rm *.o


# Makefile for Windows
SRCDIR=./source
CC=gcc
INCLUDE = C:\msys64\mingw64\include\SDL2

CFLAGS = -g -I$(INCLUDE) -I./model -c
LDFLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

# Target executable
simpleSDLexample1: main.o model.o view.o controller.o
	$(CC) main.o model.o view.o controller.o -o simpleSDLexample1 $(LDFLAGS)

# Compile main.c into main.o
main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -o main.o $(SRCDIR)/main.c

# Compile model.c into model.o
model.o: model/model.c
	$(CC) $(CFLAGS) -o model.o model/model.c

# Compile view.c into view.o
view.o: view/view.c
	$(CC) $(CFLAGS) -o view.o view/view.c

# Compile controller.c into controller.o
controller.o: controller/controller.c
	$(CC) $(CFLAGS) -o controller.o controller/controller.c

	
# Compile controller.c into controller.o
define.o: source/define.h
	$(CC) $(CFLAGS) -o define.o source/define.h

# Alla andra filer

# Clean up command
clean:
	rm -f *.exe *.o
