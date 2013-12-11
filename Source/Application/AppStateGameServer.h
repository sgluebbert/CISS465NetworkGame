#ifndef APPSTATEGAMESERVER_H
#define APPSTATEGAMESERVER_H


#include "AppStateBase.h"
#include "../Networking/Network.h"
#include "../CollisionManager.h"
#include "../Client.h"
#include "../Clock.h"
#include "../Map.h"
#include "../System.h"
#include "../Planet.h"

#include <deque>
#include <vector>
#include <bitset>
 

enum DebugLevel { DL_NONE, DL_LOW, DL_MED, DL_HIGH };

// After how much inactivity should the server kill itself?
const int TimeoutInSeconds = 30;
 
 
class AppStateGameServer : public AppStateBase {
private:
        static AppStateBase * instance;

	Network *networkGame;
        Network *networkMainServer;
        int tryMainAgain;

	DebugLevel debugLevel;
	time_t lastSpeedDisplay, lastActivity, lastSaveStats;
	Client *clients[MaximumClients];
        std::vector<Client*> expectedClients;
	int clientCount;
        bool inLobby;

        time_t secondsToStartLastTick;
        unsigned char secondsToStart;
        int teamRedCount, teamBlueCount;
        Team winner;
        GameServerEnums state;
        std::bitset<MaximumClients> availablePlayerIds;

        Map *map;
        std::string lobbyName;

        unsigned char GetNextPlayerId();
        void HandleLobbyConnections();
        void UpdateLobby();
        void SendLobbyPlayersToAll();
        bool MakeTeamsEven(int id = -1);
        void SendStateUpdate(int id = -1);

        void HandleGameConnections();
        void UpdateGameConnections();
        void SendToAll(NetString*);
        void SwitchToGameMode();
        void UpdateGame();

        void UpdateMainServer();
        
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