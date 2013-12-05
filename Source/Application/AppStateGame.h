#ifndef APPSTATEGAME_H
#define APPSTATEGAME_H



#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <deque>

#include "AppStateBase.h"
#include "AppStateGameServer.h"
#include "../Camera.h"
#include "../Client.h"
#include "../Clock.h"
#include "../CollisionManager.h"
#include "../Map.h"
#include "../System.h"
#include "../Planet.h"



class AppStateGame : public AppStateBase {
private:
        static AppStateBase * instance;
        AppStateGame();
        
        Texture * background_texture;
        Rect<double> background_rect;
        Network *network;
        
        // static const char * MUSIC_FILENAME;
        void Send();
        void Receive();
        Client player;
        Ship *players[MaximumClients];
        bool requestingGreeting;
        Map *map;

        time_t secondsToStartLastTick, secondsToEndLastTick, secondsSinceLastMessageTick;
        int secondsToStart, secondsToEnd, secondsSinceLastMessage;
        Team teamWon;
public:
 
        void Initialize();
        void Events(SDL_Event *);
        void Update();
        void Draw();
        void Cleanup();
        
        static AppStateBase * GetInstance();
 
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
};



#endif
