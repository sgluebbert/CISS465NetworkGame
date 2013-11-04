CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

all: build Source/Server.o
	$(CC) Main.cpp ./Source/*.o ../Source/*.o ../Source/Networking/*.o $(CPPFLAGS)

build:
	@cd ..;\
	make build

Server.o: Source/Server.cpp Source/Server.h

clean:
	@cd ..;\
	make clean
	rm -f ./Source/*.o
	rm -f a.out

run:
	./a.out

r:
	./a.out
