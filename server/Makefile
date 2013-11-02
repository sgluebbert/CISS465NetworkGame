CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

all: build
	$(CC) Main.cpp ../Source/*.o ../Source/Networking/*.o $(CPPFLAGS)

build:
	@cd ..;\
	make build

clean:
	@cd ..;\
	make clean
	
c:
	rm -f a.out

run:
	./a.out

r:
	./a.out
