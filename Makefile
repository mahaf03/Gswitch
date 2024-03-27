SRCDIR=./src
CC=gcc
# INCLUDE = ./lib/macos/include
INCLUDE = C:\msys64\mingw64\include

#CFLAGS = -g -I$(INCLUDE) -c
CFLAGS = -g -c

# Path to the libraries
#LIBS = C:\msys64\mingw64\lib

# Extra flags to give to compilers when they are supposed to invoke the linker
# NOTE ORDER OF THE FLAGS MATTERS!
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -mwindows

hello:
	@echo "Building Hello"
	$(CC) $(CFLAGS) hello.c -o $@.o 
	$(CC) hello.o -o hello $(LDFLAGS)

clean:
	rm hello
	rm hello.o