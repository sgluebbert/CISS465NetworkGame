CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

build: Source/TextureManager.o Source/Texture.o Source/Text.o Source/Draw.o Source/Parser.o

ApplicationEvent.o: Source/ApplicationEvent

FontManager.o: Source/FontManager.h

TextureManager.o: Source/TextureManager.cpp Source/TextureManager.h

Text.o: Source/Text.cpp Source/Text.h

Texture.o: Source/Texture.cpp Source/Texture.h

Draw.o: Source/Draw.cpp Source/Draw.h

Parser.o: Source/Parser.cpp Source/Parser.h

clean:
	rm -f client/a.out
	rm -f server/a.out
	rm -f Source/*.o

push:
	make clean
	git add *
	git status
	@echo ""
	@read -p "Commit Comment: " MSG;\
	git commit -m "$$MSG"
	git push