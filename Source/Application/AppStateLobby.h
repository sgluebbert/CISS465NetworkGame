#ifndef APPSTATELOBBY
#define APPSTATELOBBY


#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <deque>

#include "AppStateBase.h"
#include "AppStateGameServer.h"
#include "../Camera.h"
#include "../Client.h"
#include "../Clock.h"
#include "../System.h"
#include "../Map.h"



class AppStateLobby : public AppStateBase {
private:
        static AppStateBase * instance;
        AppStateLobby();
        
        // Texture * background_texture;
        // Rect<double> background_rect;
        Network *network;
        
        void Send();
        void Receive();
        Client player;
        Client *clients[MaximumClients];
        GameServerEnums state;
        Map *map;
        bool lobbyIsReady;

        time_t secondsToStartLastTick;
        int secondsToStart;
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
