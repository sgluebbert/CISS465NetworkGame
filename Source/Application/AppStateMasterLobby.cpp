#include "AppStateMasterLobby.h"


AppStateBase * AppStateMasterLobby::instance = NULL;



AppStateMasterLobby::AppStateMasterLobby()
{
	channelId = -1;
	for (int i = 0; i < MaximumClients; ++i)
		lobbies[i] = NULL;
	highlightedOption = -2;
	lobbyCount = 0;
    usernameInput = true;
    shiftOn = false;
    authenticating = false;

    createLobby = false;
    lobbySize = 0;
    lobbyButton = 0;
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
    if (!authenticated)
    {

    }

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
    DrawRect(16, 48, viewport.w - 16, viewport.h - 16, true, &color);
    color = Color(0, 0, .6, .6);
    DrawRect(16, 48, viewport.w - 16, viewport.h - 16, false, &color);
    color = Color(0, 0, 0, .5);
    DrawRect(22, 74, viewport.w - 22, viewport.h - 22, true, &color);
    color = Color(0, 0, .6, .6);
    DrawRect(22, 74, viewport.w - 22, viewport.h - 22, false, &color);

    int tableWidth = viewport.w - 32;

    Text title("Lobby Name", textureManager->fonts.font_FreeMono_14, WHITE);
    int nameX = 24, nameWidth = tableWidth * .2;
    title.Draw(nameX + nameWidth / 2 - title.width / 2, 50);

    int gameStateX = nameX + nameWidth + 4;
    int gameStateWidth = tableWidth * .4;
    title.Reload("Lobby State");
    title.Draw(gameStateX + gameStateWidth / 2 - title.width / 2, 50);

    int minPlayersCountX = gameStateX + gameStateWidth + 2;
    int minPlayersCountWidth = tableWidth * .12;
    title.Reload("Min Players");
    title.Draw(minPlayersCountX + minPlayersCountWidth / 2 - title.width / 2, 50);

    int playersCountX = minPlayersCountX + minPlayersCountWidth + 2;
    int playersCountWidth = tableWidth * .12;
    title.Reload("Players");
    title.Draw(playersCountX + playersCountWidth / 2 - title.width / 2, 50);

    int maxPlayersCountX = playersCountX + playersCountWidth + 2;
    int maxPlayersCountWidth = tableWidth * .12;
    title.Reload("Max Players");
    title.Draw(maxPlayersCountX + maxPlayersCountWidth / 2 - title.width / 2, 50);

    if (lobbyCount != 0)
    {
    	bool highlightValid = false;
	    std::stringstream stream;
	    int yoffset = 76;
	    Text row(" ", textureManager->fonts.font_FreeMono_14, WHITE);
	    Lobby *lobby;
	    for (int i = 0; i < MaximumClients; ++i)
	    {
	    	lobby = lobbies[i];
	    	if (lobby == NULL)
	    		continue;

	    	if (i == highlightedOption)
	    	{
	    		highlightValid = true;
			    color = Color(.3, .3, 1, .6);
			    DrawRect(22, yoffset, viewport.w - 22, yoffset + 22, false, &color);
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

	    if (!highlightValid && highlightedOption >= 0)
	    {
	    	// Automatically select the first available lobby.
	    	for (int i = 0; i < MaximumClients; ++i)
		    {
		    	if (lobbies[i] != NULL)
		    	{
		    		highlightedOption = i;
		    		break;
		    	}
		    }
		}
	}

    Text buttonText("Go Back", textureManager->fonts.font_FreeMono_16, WHITE);
    color = Color(0, 0, .3, .4);
    DrawRect(16, 8, 108, 40, true, &color);
    if (highlightedOption == -2)
        color = Color(.3, .3, 1, .6);
    else
        color = Color(0, 0, .6, .6);
    DrawRect(16, 8, 108, 40, false, &color);
    buttonText.Draw(25, 13);


    buttonText.Reload("Create Lobby");
    color = Color(0, 0, .3, .4);
    DrawRect(viewport.w - 152, 8, viewport.w - 16, 40, true, &color);
    if (highlightedOption == -1)
        color = Color(.3, .3, 1, .6);
    else
        color = Color(0, 0, .6, .6);
    DrawRect(viewport.w - 152, 8, viewport.w - 16, 40, false, &color);
    buttonText.Draw(viewport.w - 25 - buttonText.width, 13);
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Draw login
    if (!authenticated)
    {
        Rect<int> loginBounds(viewport.w / 2 - 200, viewport.h / 2 - 38, 400, 76);

        color = Color(0, 0, .3, .4);
        DrawRect(loginBounds.x, loginBounds.y, loginBounds.x + loginBounds.w, loginBounds.y + loginBounds.h, true, &color);
        color = Color(.3, .3, 1, .6);
        DrawRect(loginBounds.x, loginBounds.y, loginBounds.x + loginBounds.w, loginBounds.y + loginBounds.h, false, &color);

        if (!usernameInput)
            color = Color(1, 1, 1, .6);
        else
            color = WHITE;
        DrawText(loginBounds.x + 10, loginBounds.y + 2, "Username:", textureManager->fonts.font_FreeMono_20, &color);
        if (usernamePrompt.length() > 0)
            DrawText(loginBounds.x + 130, loginBounds.y + 2, usernamePrompt.c_str(), textureManager->fonts.font_FreeMono_20, &color);

        if (usernameInput)
            color = Color(1, 1, 1, .6);
        else
            color = WHITE;
        DrawText(loginBounds.x + 10, loginBounds.y + 32, "Password:", textureManager->fonts.font_FreeMono_20, &color);
        if (passwordPrompt.length() > 0)
        {
            std::stringstream temp;
            for (int i = 0; i < passwordPrompt.length(); ++i)
                temp << "*";
            DrawText(loginBounds.x + 130, loginBounds.y + 32, temp.str().c_str(), textureManager->fonts.font_FreeMono_20, &color);
        }

        if (authenticating)
        {
            color = Color(0, 0, 0, .4);
            DrawRect(loginBounds.x, loginBounds.y, loginBounds.x + loginBounds.w, loginBounds.y + loginBounds.h, true, &color);
        }
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Draw login
    if (createLobby)
    {
        Rect<int> createBounds(viewport.w / 2 - 200, viewport.h / 2 - 50, 400, 100);

        color = Color(0, 0, .3, .4);
        DrawRect(createBounds.x, createBounds.y, createBounds.x + createBounds.w, createBounds.y + createBounds.h, true, &color);
        color = Color(.3, .3, 1, .6);
        DrawRect(createBounds.x, createBounds.y, createBounds.x + createBounds.w, createBounds.y + createBounds.h, false, &color);

        if (lobbyButton == 0)
            color = WHITE;
        else
            color = Color(1, 1, 1, .6);
        DrawText(createBounds.x + 10, createBounds.y + 2, "Title:", textureManager->fonts.font_FreeMono_20, &color);
        if (lobbyNamePrompt.length() > 0)
            DrawText(createBounds.x + 90, createBounds.y + 2, lobbyNamePrompt.c_str(), textureManager->fonts.font_FreeMono_20, &color);

        if (lobbyButton == 1)
            color = WHITE;
        else
            color = Color(1, 1, 1, .6);
        DrawText(createBounds.x + 10, createBounds.y + 32, "Size:", textureManager->fonts.font_FreeMono_20, &color);

        DrawText(createBounds.x + 90, createBounds.y + 34, "Small", textureManager->fonts.font_FreeMono_18, &color);
        DrawText(createBounds.x + 200, createBounds.y + 34, "Medium", textureManager->fonts.font_FreeMono_18, &color);
        DrawText(createBounds.x + 324, createBounds.y + 34, "Large", textureManager->fonts.font_FreeMono_18, &color);

        color = Color(.3, .3, 1, .6);
        if (lobbySize == 0)
            DrawRect(createBounds.x + 80, createBounds.y + 36, createBounds.x + 160, createBounds.y + 60, false, &color);
        else if (lobbySize == 1)
            DrawRect(createBounds.x + 190, createBounds.y + 36, createBounds.x + 280, createBounds.y + 60, false, &color);
        else
            DrawRect(createBounds.x + 314, createBounds.y + 36, createBounds.x + 388, createBounds.y + 60, false, &color);

        if (lobbyButton == 2)
            color = WHITE;
        else
            color = Color(1, 1, 1, .6);
        DrawText(createBounds.x + 10, createBounds.y + 62, "Cancel", textureManager->fonts.font_FreeMono_20, &color);

        if (lobbyButton == 3)
            color = WHITE;
        else
            color = Color(1, 1, 1, .6);
        DrawText(createBounds.x + 320, createBounds.y + 62, "Create", textureManager->fonts.font_FreeMono_20, &color);
    }
    ///////////////////////////////////////////////////////////////////////////
}

void AppStateMasterLobby::Login() {
    if (authenticating)
        return;

    if (usernamePrompt.length() == 0)
        return;

    authenticating = true;

    NetString stream;
    stream.WriteUChar(NCE_PLAYER_LOGIN);
    stream.WriteString(usernamePrompt);
    stream.WriteString(passwordPrompt);
    stream.WriteUChar(NCE_END);
    network->SendData(&stream, 0);
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

            case NCE_PLAYER_LOGIN:
            {
                bool pass;
                netString.ReadBool(pass);
                if (pass)
                {
                    if (usernamePrompt == "test" || usernamePrompt == "Test")
                    {
                        std::stringstream temp;
                        temp << usernamePrompt << rand();
                        username = temp.str();
                    }
                    else
                        username = usernamePrompt;
                    authenticated = true;
                }

                authenticating = false;
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
            	if (highlightedOption == -1)
            		highlightedOption = lobby->channelId;
            	lobbyCount++;
            	// std::cout << "Got lobby: " << lobby->name << " " << lobby->gamePort << '\n';
            	break;
            }

            case NCE_CREATE_LOBBY:
            {
                std::string name;
                int port;
                netString.ReadString(name);
                netString.ReadInt(port);

                // Could redirect player to new lobby...
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
    if (!authenticated)
        return;

    if (createLobby)
    {
        if (lobbyButton > 0)
            lobbyButton--;
        return;
    }

	if (highlightedOption == -2)
		return;

    if (highlightedOption <= 0)
    {
        highlightedOption--;
        return;
    }

	int newHighlight = highlightedOption - 1;
	while (lobbies[newHighlight] == NULL)
	{
		newHighlight--;
		if (newHighlight == -1)
			return;
	}

	highlightedOption = newHighlight;
}

void AppStateMasterLobby::MoveDown() {
    if (!authenticated)
        return;

    if (createLobby)
    {
        if (lobbyButton < 3)
            lobbyButton++;
        return;
    }
    
    if (highlightedOption == MaximumClients - 1)
		return;

    if (highlightedOption == -2)
    {
        highlightedOption++;
        return;
    }

	int newHighlight = highlightedOption + 1;
	while (lobbies[newHighlight] == NULL)
	{
		newHighlight++;
		if (newHighlight == MaximumClients)
        {
            highlightedOption = -2;
			return;
        }
	}

	highlightedOption = newHighlight;
}

void AppStateMasterLobby::MakeSelection() {
    if (createLobby)
    {
        if (lobbyButton == 2)
        {
            createLobby = false;
            lobbyNamePrompt = std::string();
            lobbySize = 0;
        }
        else if (lobbyButton == 3)
        {
            if (lobbyNamePrompt.length() == 0)
                return;
            
            NetString string;
            string.WriteUChar(NCE_CREATE_LOBBY);
            std::string name = lobbyNamePrompt;
            float size = 1.5;
            if (lobbySize == 1)
                size = 2.0;
            else if (lobbySize == 2)
                size = 2.5;
            string.WriteString(name);
            string.WriteFloat(size);
            string.WriteUChar(NCE_END);
            network->SendData(&string, 0);

            createLobby = false;
            lobbyNamePrompt = std::string();
            lobbySize = 0;
        }

        return;
    }

	if (highlightedOption == -2)
    {
        AppStateEvent::New_Event(APPSTATE_MENU);
        return;
    }
    
    if (highlightedOption == -1)
    {
        createLobby = true;
		return;
    }

	directedGamePort = lobbies[highlightedOption]->gamePort;
    if ((int)lobbies[highlightedOption]->address.host == LocalhostIntRepresentation)
        directedGameHost = network->GetSendHost();
    else
       directedGameHost = lobbies[highlightedOption]->address.host;
	directedLobbyName = lobbies[highlightedOption]->name;
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
        case SDLK_UP:           MoveUp(); usernameInput = !usernameInput;       break;
        case SDLK_DOWN:         MoveDown(); usernameInput = !usernameInput;		break;
        case SDLK_LEFT:         if (createLobby && lobbySize > 0 && lobbyButton == 1) lobbySize--;  break;
        case SDLK_RIGHT:        if (createLobby && lobbySize < 2 && lobbyButton == 1) lobbySize++;  break;
        case SDLK_ESCAPE:       AppStateEvent::New_Event(APPSTATE_MENU);        break;
        case SDLK_TAB:          usernameInput = !usernameInput; MoveDown();     break;
        case SDLK_RSHIFT:       shiftOn = true;         break;
        case SDLK_LSHIFT:       shiftOn = true;         break;
        case SDLK_RETURN:
            if (authenticating)
                break;
            if (!authenticated)
                Login();
            else
                MakeSelection();
            break;
        case SDLK_BACKSPACE:
            if (authenticating)
                break;
            if (!authenticated)
            {
                if (usernameInput && usernamePrompt.length() > 0)
                    usernamePrompt = usernamePrompt.substr(0, usernamePrompt.length() - 1);
                else if (passwordPrompt.length() > 0)
                    passwordPrompt = passwordPrompt.substr(0, passwordPrompt.length() - 1);
            }
            else if (createLobby)
            {
                if (lobbyNamePrompt.length() > 0 && lobbyButton == 0)
                    lobbyNamePrompt = lobbyNamePrompt.substr(0, lobbyNamePrompt.length() - 1);
            }
            break;
        default:
        {
            if (authenticating)
                break;
            if (!authenticated)
            {
                char key = ConvertToChar(sym, shiftOn);
                if (key != 0)
                {
                    if (usernameInput)
                        usernamePrompt += key;
                    else
                        passwordPrompt += key;
                }
            }
            else if (createLobby && lobbyButton == 0)
            {
                char key = ConvertToChar(sym, shiftOn);
                if (key != 0)
                {
                    lobbyNamePrompt += key;
                }
            }
            break;
        }
    }
}

void AppStateMasterLobby::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
        case SDLK_RSHIFT:       shiftOn = false;         break;
        case SDLK_LSHIFT:       shiftOn = false;         break;
        default:    break;
    }
}
