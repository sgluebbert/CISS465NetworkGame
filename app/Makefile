CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

all: build
	$(CC) Main.cpp ../Source/*.o ../Source/*/*.o -o starclash $(CPPFLAGS)

build:
	@cd ..;\
	make build

run:
	./starclash

r:
	./starclash

clean:
	@cd ..;\
	make clean
	rm -f Source/*.o

c:
	#rm a.out *.cpp~ ./*.h~ ./*.cpp~

