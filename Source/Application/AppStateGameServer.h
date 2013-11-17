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
#include <bitset>
 

enum DebugLevel { DL_NONE, DL_LOW, DL_MED, DL_HIGH };
enum GameServerEnums { GSE_WAITING, GSE_LOBBY_COUNTDOWN, GSE_TRANSITION, GSE_GAME_COUNTDOWN, GSE_GAME };
 
 
class AppStateGameServer : public AppStateBase {
private:
        static AppStateBase * instance;

	Network *network;
	DebugLevel debugLevel;
	time_t lastSpeedDisplay;
	Client *clients[MaximumClients];
        std::vector<Client*> expectedClients;
	int clientCount;
        bool inLobby;

        time_t secondsToStartLastTick;
        unsigned char secondsToStart;
        int team1Count, team2Count;
        GameServerEnums state;
        std::bitset<MaximumClients> availablePlayerIds;

        Map *map;

        unsigned char GetNextPlayerId();
        void HandleLobbyConnections();
        void UpdateLobby();
        void SendLobbyPlayersToAll();
        void MakeTeamsEven(int id = -1);
        void SendStateUpdate(int id = -1);

        void HandleGameConnections();
        void UpdateGameConnections();
        void SendToAll(NetString*);
        void SwitchToGameMode();
        void UpdateGame();
        
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