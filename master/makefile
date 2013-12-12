CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_net `mysql_config --cflags --libs`

all:
	@cd ..;\
	make build
	make build

build: ./Source/HeadServer.o
	$(CC) main.cpp ./Source/*.o ../Source/Networking/*.o -o mainserver $(CPPFLAGS)

HeadServer.o: ./Source/HeadServer.cpp ./Source/HeadServer.h

publish:
	@read -p "bilbo.ccis.edu username: " USERNAME;\
	ssh $$USERNAME@bilbo.ccis.edu "rm -rf ~/ciss465/master/; rm -rf ~/ciss465/app/; mkdir -p ~/ciss465/master; mkdir -p ~/ciss465/app;";\
	scp -r mainserver conf $$USERNAME@bilbo.ccis.edu:~/ciss465/master/;\
	scp -r ../app/starclash ../app/conf $$USERNAME@bilbo.ccis.edu:~/ciss465/app/;\

clean:
	rm -f mainserver ./Source/*.o