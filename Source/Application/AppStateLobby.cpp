#include "AppStateLobby.h"


AppStateBase * AppStateLobby::instance = NULL;



AppStateLobby::AppStateLobby()
	: secondsToStart(100), state(GSE_WAITING), lobbyIsReady(false), stateText("Waiting for additional players...", TextureManager::GetInstance()->fonts.font_FreeMono_18, WHITE),
      team1Count(0), team2Count(0)
{
	time(&secondsToStartLastTick);
	for (int i = 0; i < MaximumClients; i++)
		clients[i] = NULL;
	map = new Map(0);
}

void AppStateLobby::Initialize() {
	player.player_name = Get_Username();
	std::cout << "I am " << player.player_name << '\n';

	network = NetworkFactory::getInstance("./conf/networkLobby.conf");
	if (network->Init() == -1)
	{
		// LobbyServer is not listening... Try the actual game server on udp
		AppStateEvent::New_Event(APPSTATE_GAME);
		return;
	}

	lobbyIsReady = true;

	NetString string;
	string.WriteUChar(NCE_PLAYER);
	string.WriteString(player.player_name);
	string.WriteUChar(NCE_END);
	network->SendData(&string, 0);
}

void AppStateLobby::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateLobby::Update() {
	if (!lobbyIsReady)
		return;

    Receive();

    if (state == GSE_LOBBY_COUNTDOWN)
    {
		time_t now;
		time(&now);
		if (now - secondsToStartLastTick >= 1)
		{
			time(&secondsToStartLastTick);
			secondsToStart--;
            std::stringstream stream;
            stream << "Seconds to start: " << (int)secondsToStart;
            stateText.Reload(stream.str().c_str());
			std::cout << stream.str() << '\n';
		}
	}
}

void AppStateLobby::Draw() {
	if (!lobbyIsReady)
		return;

    Rect<double> viewport = Camera::getInstance()->Get_Viewport();

    ////////////////////////////////////////////////////////////////////////////
    // Draw players table
    TextureManager *textureManager = TextureManager::GetInstance();

    int drawx = viewport.w - 300;
    int drawy = 50;

    DrawText(drawx, drawy - 22, "Team Red", textureManager->fonts.font_Impact_20, &WHITE);

    // Draw all team 0 players; not the fastest way, but we don't need speed.
    int offset = 0;
    for (int i = 0; i < MaximumClients; i++)
    {
        if (clients[i] == NULL || clients[i]->team_id != 0)
            continue;

        // Add other things like exp lvl?
        std::stringstream stream;
        stream << clients[i]->player_name;
        DrawText(drawx, drawy + offset * 18, stream.str().c_str(), textureManager->fonts.font_FreeMono_16, &WHITE);
        offset++;
    }

    drawy += offset * 18 + 40;
    DrawText(drawx, drawy - 22, "Team Blue", textureManager->fonts.font_Impact_20, &WHITE);

    // Draw all team 1 players
    offset = 0;
    for (int i = 0; i < MaximumClients; i++)
    {
        if (clients[i] == NULL || clients[i]->team_id != 1)
            continue;

        // Add other things like exp lvl?
        std::stringstream stream;
        stream << clients[i]->player_name;
        DrawText(drawx, drawy + offset * 18, stream.str().c_str(), textureManager->fonts.font_FreeMono_16, &WHITE);
        offset++;
    }
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Draw Map
    DrawRect(10, 10, 480, 400, false, &WHITE);
    std::stringstream stream;
    stream << "Map seed: " << map->SEED;
    DrawText(14, 14, stream.str().c_str(), textureManager->fonts.font_FreeMono_16, &WHITE);
    ////////////////////////////////////////////////////////////////////////////

    stateText.Draw(viewport.w / 2 - stateText.width / 2, viewport.h - stateText.height - 10);
}

void AppStateLobby::Receive() {
	NetString netString;
    while (network->ReceiveData() != -1)
    {
        NetString *temp = network->GetData();
        if (temp != NULL)
            netString += *temp;
        delete temp;
    }

    netString.Seek(0);
    if (netString.BufferLength() == 0)
        return;

    // std::cout << "Got: " << netString << '\n';

    bool reading = true;
    while (reading)
    {
        NetworkChunkEnums type;
        unsigned char temp;
        if (!netString.ReadUChar(temp))
            break;
        type = (NetworkChunkEnums)temp;

        switch (type)
        {
        	case NCE_TOO_MANY_PLAYERS:
        		std::cout << "Too Many Players\n";

        		break;

            case NCE_NEW_CONNECTION:
            {
            	unsigned char channelId;
                netString.ReadUChar(channelId);

                if (player.channel_id == -1)
                {
                    player.channel_id = channelId;
                    clients[channelId] = &player;
                }
                break;
            }

            case NCE_MAP:
            	int temp1;
            	netString.ReadInt(temp1);
            	delete map;
            	map = new Map(temp1);
            	std::cout << "Map Seed: " << temp1 << '\n';

            case NCE_LOBBY_PLAYER_SYNC:
            	for (int i = 0; i < MaximumClients; i++)
            	{
            		if (i != player.channel_id)
            		{
            			delete clients[i];
            			clients[i] = NULL;
            		}
            	}

                team1Count = 0;
                team2Count = 0;
            	break;

            case NCE_PLAYER:
            {
            	unsigned char playerId;
                netString.ReadUChar(playerId);

                if (clients[playerId] == NULL)
                {
                	clients[playerId] = new Client();
                	clients[playerId]->offline = false;
                }

                int temp1;
                netString.ReadInt(temp1);
                clients[playerId]->team_id = temp1;
                if (temp1 == 0)
                    team1Count++;
                else
                    team2Count++;
                netString.ReadInt(temp1);
                clients[playerId]->player_id = temp1;
				netString.ReadString(clients[playerId]->player_name);

                std::cout << "Player: " << clients[playerId]->player_name << "; Id: " << (int)clients[playerId]->player_id << "; Team: " << (int)clients[playerId]->team_id << '\n';
                EvaluateNeededPlayers();
                break;
            }

            case NCE_REMOVE_PLAYER:
            	unsigned char playerId;
                netString.ReadUChar(playerId);
                if (clients[playerId] != NULL && clients[playerId] != &player)
                {
                    delete clients[playerId];
                    clients[playerId] = NULL;
                }

                EvaluateNeededPlayers();
                break;

            case NCE_LOBBY_STATE_SYNC:
            {
            	GameServerEnums serverState;
				unsigned char temp;
				if (!netString.ReadUChar(temp))
					break;
				serverState = (GameServerEnums)temp;

				switch (serverState)
				{
					case GSE_WAITING:
						state = GSE_WAITING;
                        EvaluateNeededPlayers();
						break;

					case GSE_LOBBY_COUNTDOWN:
					{
						state = GSE_LOBBY_COUNTDOWN;
						int tempI;
						unsigned char tempC;
						netString.ReadUChar(tempC);
						secondsToStart = tempC;
						netString.ReadInt(tempI);
						secondsToStartLastTick = tempI;
                        std::stringstream stream;
						stream << "Seconds to start: " << (int)secondsToStart;
                        stateText.Reload(stream.str().c_str());
                        std::cout << stream.str() << '\n';
						break;
					}

					case GSE_TRANSITION:
						AppStateEvent::New_Event(APPSTATE_GAME);
						break;
				}
            	break;
            }

            case NCE_START_GAME_TIMER:
                int tempI;
                unsigned char tempC;
                netString.ReadUChar(tempC);
                secondsToStart = tempC;
                netString.ReadInt(tempI);
                secondsToStartLastTick = tempI;
                break;

            case NCE_END:
            	// std::cout << "End\n";
            	break;

            default:
            	std::cout << "Unrecognized Type: " << type << '\n';
                reading = false;
        }
    }
}

void AppStateLobby::EvaluateNeededPlayers() {
    if (state != GSE_WAITING)
        return;
    std::stringstream stream;
    int needed = map->MIN_NUMBER_OF_PLAYERS_PER_TEAM * 2 - (team1Count + team2Count);
    stream << "Waiting for " << needed << " additional players...";
    stateText.Reload(stream.str().c_str());
    std::cout << stream.str() << '\n';\
}

void AppStateLobby::Cleanup() {
	network->Close();
	delete network;
}

AppStateBase * AppStateLobby::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateLobby();
    return instance;
}

void AppStateLobby::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:    break;
    }
}

void AppStateLobby::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:    break;
    }
}
