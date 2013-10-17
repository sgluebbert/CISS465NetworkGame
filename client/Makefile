CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

all: Source/SurfaceManager.o
	g++ -g Main.cpp Source/SurfaceManager.o $(CPPFLAGS)

# Others.o: Main.cpp
# 	$(CC) $(CPPFLAGS) Main.cpp

ApplicationEvent.o: Source/ApplicationEvent

FontManager.o: Source/FontManager.h

SurfaceManager.o: Source/SurfaceManager.cpp Source/SurfaceManager.h

# all: Main.cpp
# 	clang++ Main.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

run:
	./a.out

r:
	./a.out

clean:
	rm a.out Source/*.o

c:
	#rm a.out *.cpp~ ./*.h~ ./*.cpp~

