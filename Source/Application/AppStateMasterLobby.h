#ifndef APPSTATEMASTERLOBBY
#define APPSTATEMASTERLOBBY


#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <deque>

#include "AppStateBase.h"
#include "AppStateGameServer.h"
#include "../Camera.h"
#include "../Clock.h"
#include "../System.h"
#include "../Map.h"
#include "../Networking/Lobby.h"


class AppStateMasterLobby : public AppStateBase {
private:
        static AppStateBase * instance;
        AppStateMasterLobby();
        
        Network *network;
        
        void Send();
        void Receive();
        void MoveUp();
        void MoveDown();
        void MakeSelection();

        Lobby *lobbies[MaximumClients];
        int highlightedLobby;
        int lobbyCount;
        char channelId;
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