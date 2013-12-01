#include "AppStateMasterLobby.h"


AppStateBase * AppStateMasterLobby::instance = NULL;



AppStateMasterLobby::AppStateMasterLobby()
{
	channelId = -1;
	for (int i = 0; i < MaximumClients; ++i)
		lobbies[i] = NULL;
	highlightedLobby = -1;
	lobbyCount = 0;
}

void AppStateMasterLobby::Initialize() {
	// player.player_name = Get_Username();
	// std::cout << "I am " << player.player_name << '\n';

	network = NetworkFactory::getInstance("./conf/networkMasterLobby.conf");
	if (network->Init() == -1)
	{
		std::cerr << "Unable to connect to master server.\n";
		AppStateEvent::New_Event(APPSTATE_MENU);
		network = NULL;
		return;
	}

	// NetString string;	
	// string.WriteUChar(NCE_PLAYER);
	// string.WriteString(player.player_name);
	// string.WriteUChar(NCE_END);
	// network->SendData(&string, 0);
}

void AppStateMasterLobby::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateMasterLobby::Update() {

    Receive();
}

void AppStateMasterLobby::Draw() {
    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    TextureManager *textureManager = TextureManager::GetInstance();

    glColor4f(1, 1, 1, 1);
    textureManager->background_game2->DrawAtRect(0, 0, 1024, 1024);

    ///////////////////////////////////////////////////////////////////////////
    // Draw lobby table
    Color color(0, 0, .3, .4);
    DrawRect(16, 16, viewport.w - 16, viewport.h - 48, true, &color);
    color = Color(0, 0, .6, .6);
    DrawRect(16, 16, viewport.w - 16, viewport.h - 48, false, &color);
    color = Color(0, 0, 0, .5);
    DrawRect(22, 42, viewport.w - 22, viewport.h - 54, true, &color);
    color = Color(0, 0, .6, .6);
    DrawRect(22, 42, viewport.w - 22, viewport.h - 54, false, &color);

    int tableWidth = viewport.w - 32;

    Text title("Lobby Name", textureManager->fonts.font_FreeMono_14, WHITE);
    int nameX = 28, nameWidth = tableWidth * .2;
    title.Draw(nameX + nameWidth / 2 - title.width / 2, 16);

    int gameStateX = nameX + nameWidth + 4;
    int gameStateWidth = tableWidth * .4;
    title.Reload("Lobby State");
    title.Draw(gameStateX + gameStateWidth / 2 - title.width / 2, 16);

    int minPlayersCountX = gameStateX + gameStateWidth + 2;
    int minPlayersCountWidth = tableWidth * .12;
    title.Reload("Min Players");
    title.Draw(minPlayersCountX + minPlayersCountWidth / 2 - title.width / 2, 16);

    int playersCountX = minPlayersCountX + minPlayersCountWidth + 2;
    int playersCountWidth = tableWidth * .12;
    title.Reload("Players");
    title.Draw(playersCountX + playersCountWidth / 2 - title.width / 2, 16);

    int maxPlayersCountX = playersCountX + playersCountWidth + 2;
    int maxPlayersCountWidth = tableWidth * .12;
    title.Reload("Max Players");
    title.Draw(maxPlayersCountX + maxPlayersCountWidth / 2 - title.width / 2, 16);

    if (lobbyCount == 0)
    	highlightedLobby = -1;
    else
    {
    	bool highlightValid = false;
	    std::stringstream stream;
	    int yoffset = 44;
	    Text row(" ", textureManager->fonts.font_FreeMono_14, WHITE);
	    Lobby *lobby;
	    for (int i = 0; i < MaximumClients; ++i)
	    {
	    	lobby = lobbies[i];
	    	if (lobby == NULL)
	    		continue;

	    	if (i == highlightedLobby)
	    	{
	    		highlightValid = true;
			    color = Color(.3, .3, 1, .6);
			    DrawRect(22, yoffset + 2, viewport.w - 22, yoffset + 20, false, &color);
	    	}

	    	stream.str(std::string());
	    	stream << lobby->name;
	    	row.Reload(stream.str().c_str());
	    	row.Draw(nameX, yoffset);

	    	stream.str(std::string());
	    	stream << lobby->state;
	    	row.Reload(stream.str().c_str());
	    	row.Draw(gameStateX, yoffset);
	    	Map map(lobby->mapSeed, lobby->mapScale);

	    	stream.str(std::string());
	    	stream << map.min_players_per_team;
	    	row.Reload(stream.str().c_str());
	    	row.Draw(minPlayersCountX, yoffset);

	    	stream.str(std::string());
	    	stream << (int)lobby->playerCount;
	    	row.Reload(stream.str().c_str());
	    	row.Draw(playersCountX, yoffset);

	    	stream.str(std::string());
	    	stream << map.max_players_per_team;
	    	row.Reload(stream.str().c_str());
	    	row.Draw(maxPlayersCountX, yoffset);

	    	yoffset += 18;
	    }

	    if (!highlightValid)
	    {
	    	// Automatically select the first available lobby.
	    	for (int i = 0; i < MaximumClients; ++i)
		    {
		    	if (lobbies[i] != NULL)
		    	{
		    		highlightedLobby = i;
		    		break;
		    	}
		    }
		}
	}
    ///////////////////////////////////////////////////////////////////////////

}

void AppStateMasterLobby::Receive() {
	if (network == NULL)
		return;

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
				AppStateEvent::New_Event(APPSTATE_MENU);
        		break;

            case NCE_ALREADY_JOINED:
                std::cout << "You have already joined.\n";
                AppStateEvent::New_Event(APPSTATE_MENU);
                break;

            case NCE_NEW_CONNECTION:
            {
            	unsigned char tempC;
                netString.ReadUChar(tempC);
                channelId = tempC;
                break;
            }

            case NCE_LOBBY_SYNC:
            	for (int i = 0; i < MaximumClients; ++i)
            	{
            		if (lobbies[i] == NULL)
            		{
            			delete lobbies[i];
            			lobbies[i] = NULL;
            		}
            	}

            	lobbyCount = 0;
            	break;

            case NCE_LOBBY:
            {
            	Lobby *lobby = new Lobby;
            	lobby->Deserialize(&netString);
            	if (lobbies[lobby->channelId] != NULL)
            		delete lobbies[lobby->channelId];
            	lobbies[lobby->channelId] = lobby;
            	if (highlightedLobby == -1)
            		highlightedLobby = lobby->channelId;
            	lobbyCount++;
            	// std::cout << "Got lobby: " << lobby->name << '\n';
            	break;
            }

            case NCE_END:
            	// std::cout << "End\n";
            	break;

            default:
            	std::cout << "Unrecognized Type: " << type << '\n';
                reading = false;
        }
    }
}

void AppStateMasterLobby::MoveUp() {
	if (lobbyCount == 0 || highlightedLobby == 0)
		return;

	int newHighlight = highlightedLobby - 1;
	while (lobbies[newHighlight] == NULL)
	{
		newHighlight--;
		if (newHighlight == -1)
			return;
	}

	highlightedLobby = newHighlight;
}

void AppStateMasterLobby::MoveDown() {
	if (lobbyCount == 0 || highlightedLobby == MaximumClients - 1)
		return;

	int newHighlight = highlightedLobby + 1;
	while (lobbies[newHighlight] == NULL)
	{
		newHighlight++;
		if (newHighlight == MaximumClients)
			return;
	}

	highlightedLobby = newHighlight;
}

void AppStateMasterLobby::MakeSelection() {
	if (highlightedLobby == -1)
		return;

	directedGameHost = lobbies[highlightedLobby]->address.host;
	directedGamePort = lobbies[highlightedLobby]->gamePort;
	directedLobbyName = lobbies[highlightedLobby]->name;
	AppStateEvent::New_Event(APPSTATE_LOBBY);
}

void AppStateMasterLobby::Cleanup() {
	if (network != NULL)
	{
		network->Close();
		delete network;
	}

	for (int i = 0; i < MaximumClients; i++)
		delete lobbies[i];
}

AppStateBase * AppStateMasterLobby::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateMasterLobby();
    return instance;
}

void AppStateMasterLobby::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_UP:		MoveUp();				break;
    case SDLK_DOWN:		MoveDown();				break;
    case SDLK_RETURN:	MakeSelection();		break;
    case SDLK_o:        std::cout << "Memory Usage: " << Get_Memory_Usage() << "kb" << std::endl;                                   break;
    default:    break;
    }
}

void AppStateMasterLobby::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:    break;
    }
}
