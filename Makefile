# Makefile for Windows

CFLAGS = -g -I$(INCLUDE) -c 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm -lSDL2_net -lSDL2_mixer

vpath %.h ./source
vpath %.c ./source
SRCDIR=./source
CC=gcc
INCLUDE=C:\msys64\mingw64\include\SDL2

EXECUTABLE=gswitch
SERVER=server

all: $(EXECUTABLE) $(SERVER)

#making the server
server: $(SRCDIR)/server.o  $(SRCDIR)/Network.o
	$(CC) $(SRCDIR)/server.o $(SRCDIR)/Network.o -o $(SERVER) $(LDFLAGS)

# Linking all the object files to the executable

$(EXECUTABLE): $(SRCDIR)/main.o $(SRCDIR)/Network.o $(SRCDIR)/GameModel.o $(SRCDIR)/GameView.o $(SRCDIR)/GameController.o
	$(CC) $(SRCDIR)/main.o $(SRCDIR)/Network.o $(SRCDIR)/GameModel.o  $(SRCDIR)/GameView.o $ $(SRCDIR)/GameController.o -o $(EXECUTABLE) $(LDFLAGS)

# Compiling the main file into its object file
$(SRCDIR)/main.o: main.c $(SRCDIR)/GameModel.h  Network.h GameView.h GameController.h
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $(SRCDIR)/main.o $(LDFLAGS)

#compiling the server mainfile into its object file
$(SRCDIR)/server.o: server.c $(SRCDIR)/Network.h
	$(CC) $(CFLAGS) $(SRCDIR)/server.c -o $(SRCDIR)/server.o $(LDFLAGS)
# Compiling the model file into its object file
$(SRCDIR)/GameModel.o: GameModel.c GameModel.h GameView.h GameController.h
	$(CC) $(CFLAGS) $(SRCDIR)/GameModel.c -o $(SRCDIR)/GameModel.o $(LDFLAGS)

# Compiling the view file into its object file
$(SRCDIR)/GameView.o: GameView.c GameView.h GameController.h
	$(CC) $(CFLAGS) $(SRCDIR)/GameView.c -o $(SRCDIR)/GameView.o $(LDFLAGS)

# Compiling the controller file into its object file
$(SRCDIR)/GameController.o: GameController.c GameController.h
	$(CC) $(CFLAGS) $(SRCDIR)/GameController.c -o $(SRCDIR)/GameController.o $(LDFLAGS)
#compiling the network file into its object file
$(SRCDIR)/Network.o: Network.c Network.h
	$(CC) $(CFLAGS) $(SRCDIR)/Network.c -o $(SRCDIR)/Network.o $(LDFLAGS)

# Clean up command
clean:
	@echo "Cleaning..."
	-del /Q "$(SRCDIR)\*.o"
	-del /Q $(EXECUTABLE).exe
	-del /Q $(SERVER).exe
	@echo "Clean successful."