CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_net `mysql_config --cflags --libs`

all:
	@cd ..;\
	make build
	make build

build: ./Source/HeadServer.o
	$(CC) main.cpp ./Source/*.o ../Source/Networking/*.o -o mainserver $(CPPFLAGS)

HeadServer.o: ./Source/HeadServer.cpp ./Source/HeadServer.h

clean:
	rm -f mainserver ./Source/*.o