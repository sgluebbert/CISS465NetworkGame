CC = g++
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU


build: Source/Application.o Source/ApplicationEvent.o Source/AppStateEvent.o Source/AppStateBase.o Source/AppStateIntro.o Source/AppStateMenu.o Source/AppStateGame.o Source/AppStateTest.o Source/AppStateManager.o Source/Camera.o Source/Client.o Source/Clock.o Source/Collision.o Source/Draw.o Source/Entity.o Source/EventHandler.o Source/Menu.o Source/NetString.o Source/Network.o Source/Parser.o Source/Particle.o Source/Particle_Emitter.o Source/Quad_Tree.o Source/Ship.o Source/SoundManager.o Source/System.o Source/Text.o Source/Texture.o Source/TextureManager.o Source/Timer.o

Application.o: Source/Application.h Source/Application.cpp

ApplicationEvent.o: Source/ApplicationEvent.h Source/ApplicationEvent.cpp

AppStateEvent.o: Source/AppStateEvent.h Source/AppStateEvent.cpp

AppStateBase.o: Source/AppStateBase.h Source/AppStateBase.cpp

AppStateManager.o: Source/AppStateManager.h Source/AppStateManager.cpp

AppStateIntro.o: Source/AppStateIntro.h Source/AppStateIntro.cpp

AppStateMenu.o: Source/AppStateMenu.h Source/AppStateMenu.cpp

AppStateGame.o: Source/AppStateGame.h Source/AppStateGame.cpp

AppStateTest.o: Source/AppStateTest.h Source/AppStateTest.cpp

Camera.o: Source/Camera.cpp Source/Camera.h

Client.o: Source/Client.cpp Source/Client.h

Clock.o: Source/Clock.cpp Source/Clock.h

Collision.o: Source/Collision.cpp Source/Collision.h

Draw.o: Source/Draw.cpp Source/Draw.h

Entity.o: Source/Entity.cpp Source/Entity.h

EventHandler.o: Source/EventHandler.cpp Source/EventHandler.h

Menu.o: Source/Menu.cpp Source/Menu.h

NetString.o: Source/NetString.cpp Source/NetString.h

Network.o: Source/Network.cpp Source/Network.h

Parser.o: Source/Parser.cpp Source/Parser.h

Particle.o: Source/Particle.cpp Source/Particle.h

Particle_Emitter.o: Source/Particle_Emitter.cpp Source/Particle_Emitter.h

Quad_Tree.o: Source/Quad_Tree.cpp Source/Quad_Tree.h

Ship.o: Source/Ship.cpp Source/Ship.h

SoundManager.o: Source/SoundManager.cpp Source/SoundManager.h

System.o: Source/System.cpp Source/System.h

Text.o: Source/Text.cpp Source/Text.h

Texture.o: Source/Texture.cpp Source/Texture.h

TextureManager.o: Source/TextureManager.cpp Source/TextureManager.h

Timer.o: Source/Timer.cpp Source/Timer.h

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