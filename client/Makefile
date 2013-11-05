CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

all: build Source/Client.o
	$(CC) Main.cpp ./Source/*.o ../Source/*.o ../Source/Networking/*.o $(CPPFLAGS)

build:
	@cd ..;\
	make build

Client.o: Source/Client.cpp Source/Client.h

run:
	./a.out

r:
	./a.out

clean:
	@cd ..;\
	make clean
	rm -f Source/*.o

c:
	#rm a.out *.cpp~ ./*.h~ ./*.cpp~

