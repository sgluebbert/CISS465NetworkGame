#ifndef APPSTATEGAMESERVER_H
#define APPSTATEGAMESERVER_H


#include "AppStateBase.h"
#include "../Clock.h"
#include "../System.h"
#include "../Networking/Network.h"
#include "../Client.h"
#include "../Map.h"

#include <deque>
#include <vector>
 

enum DebugLevel { DL_NONE, DL_LOW, DL_MED, DL_HIGH };
 
 
class AppStateGameServer : public AppStateBase {
private:
        static AppStateBase * instance;

	Network *network;
	DebugLevel debugLevel;
	time_t lastSpeedDisplay;
	Client *clients[MaximumClients];
	int clientCount;

        time_t secondsToStartLastTick;
        unsigned char secondsToStart;

        Map map;

	void HandleConnections();
	void UpdateConnections();
        void SendSecondsToStart(int id = -1);
	void SendToAll(NetString*);
        
        AppStateGameServer(DebugLevel l = DL_MED);
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

const std::string CurrentDateTime();


#endif