CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

all: build
	$(CC) Main.cpp ../Source/TextureManager.o ../Source/Texture.o ../Source/Text.o ../Source/Draw.o $(CPPFLAGS)

build:
	@cd ..;\
	make build

run:
	./a.out

r:
	./a.out

clean:
	@cd ..;\
	make clean

c:
	#rm a.out *.cpp~ ./*.h~ ./*.cpp~

