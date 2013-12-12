# This makefile builds the main resources for the game side of the final
# project for CISS465.

# Compiler for the system. For our purposes, standard g++
CC = g++

# Any relevant options for compilation. Maybe -wall? 
CPPFLAGS = -g `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lGL -lGLU

# The directory where the object files are stored (to be stored, actually.)
# This is the same directory as the SRCDIR for now.
OBJDIR = Source

# List of all objects that this makefile is concerned with.
# Anything not immediately related to server or client is here for now ???.
OBJECTS = Application/Application.o Application/ApplicationEvent.o Application/AppStateBase.o Application/AppStateEvent.o \
          Application/AppStateGame.o Application/AppStateGameServer.o Application/AppStateLobby.o Application/AppStateMasterLobby.o \
          Application/AppStateIntro.o Application/AppStateManager.o Application/AppStateMenu.o Application/AppStateTest.o \
          Camera.o Chat_Feed.o Client.o Clock.o Collidable.o Collision.o CollisionManager.o Drawing/Drawable.o Drawing/Draw.o Entity.o \
          Application/EventHandler.o Map.o Menu.o Networking/NetString.o \
          Networking/Network.o Networking/NetworkParser.o Networking/Parser.o \
          Particles/Particle.o Particles/Particle_Emitter.o Pickup.o Pickup_Factory.o Progress_Bar.o Progress_Circle.o \
          Quad_Tree.o Radar.o RigidBody.o Ship.o System.o Drawing/Text.o Drawing/Texture.o \
          Drawing/TextureManager.o Timer.o Weapon.o Moon.o Planet.o SoundManager.o \
          HUD/HUDManager.o HUD/HUDElements.o

# The main workhorse of the makefile. Builds all the object files
# and then updates the timestamp used to update clients.
build: $(addprefix $(OBJDIR)/, $(OBJECTS))

# Just individual compilation of all objects.
$(addprefix $(OBJDIR)/, $(OBJECTS)): %.o: %.cpp
	$(CC) -c $(CPPFLAGS) $< -o $@

# This updates the timestamp that is compared with to update clients if needed.
TimeStamp: updater/source_timestamp.sh
	(cd updater && ./source_timestamp.sh)

# Cleans all the object files and executables.
.PHONY: clean
clean:
	rm -f Source/*.o
	rm -f Source/*/*.o
	rm -f app/starclash
	rm -f master/masterserver

# This gets the repo up and ready for pushing to the master origin on github.
# Make sure you see all your changes when the status is displayed.
# Please provide a succinct but descriptive commit message in the *present*
# tense because that is the standard for git commits.
push:
	make clean
	git add . -A
	git status
	@echo ""
	@read -p "Commit Comment: " MSG;\
	git commit -m "$$MSG"
	git push
