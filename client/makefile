exe: Surface.o Input.o
	g++ main.cpp Surface.o Input.o `sdl-config --cflags --libs`

Surface.o: Surface.cpp Surface.h
	g++ -c Surface.cpp `sdl-config --cflags --libs`

Input.o: Input.cpp Input.h
	g++ -c Input.cpp `sdl-config --cflags --libs`

clean:
	rm -f a.out
	rm -f *.o

run:
	./a.out

r:
	./a.out