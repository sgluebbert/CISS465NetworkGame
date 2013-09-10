exe: Surface.o Input.o
	g++ Main.cpp Surface.o Input.o `sdl-config --cflags --libs` -Iincludes#Source/*.cpp 

Surface.o: Surface.h
	g++ -c Surface.cpp `sdl-config --cflags --libs`

Input.o: Input.h
	g++ -c Input.cpp `sdl-config --cflags --libs`

clean:
	rm -f a.out
	rm -f *.o

run:
	./a.out

r:
	./a.out
