#include "AppStateGame.h"



AppStateBase * AppStateGame::instance = NULL;



AppStateGame::AppStateGame() {
    player.offline = false;
    player.player_name = username;

    Drawable::objects.clear();
    Rigid_Body::objects.clear();
    Collidable::objects.clear();

    map = new Map(0, 1.0);
    requestingGreeting = true;

    for (int i = 0; i < MaximumClients; ++i)
        players[i] = NULL;


    time(&secondsToStartLastTick);
    secondsToStart = 100;
    time(&secondsToEndLastTick);
    secondsToEnd = 5;
    time(&secondsSinceLastMessageTick);
    secondsSinceLastMessage = 0;
    teamWon = NO_TEAM;
}

void AppStateGame::Initialize() {
    if (directedGameHost == 0)
       network = NetworkFactory::getInstance();
    else
    {
        IPaddress hostaddress;
        hostaddress.host = directedGameHost;
        hostaddress.port = directedGamePort;
        const char *hostname = SDLNet_ResolveIP(&hostaddress);
        if (hostname == NULL)
        {
            std::cerr << "Could not resolve directed host\n";
            AppStateEvent::New_Event(APPSTATE_MENU);
            return;
        }
        else
            network = NetworkFactory::getInstance(UDP, hostname, directedGamePort);
    }

    network->Init();
    background_texture = surface_manager->background_game;
    background_rect = Camera::getInstance()->Get_Viewport();

    Collision_Manager::Get_Instance();
    srand(time(NULL));
    Initialize_Trig_Table();
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
    
    // std::cout << "Beginning Updates..." << std::endl;
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

    // std::cout << "Beginning Particle Culling..." << std::endl;
    for (int i = Particle::particles.size() - 1; i >= 0; i--)
        if (Particle::particles[i]->Is_Dead()) {
            for (int j = Rigid_Body::objects.size() - 1; j >= 0; j--)
                if (Particle::particles[i] == Rigid_Body::objects[j])
                    Rigid_Body::objects.erase(Rigid_Body::objects.begin() + j);
            for (int j = Drawable::objects.size() - 1; j >= 0; j--)
                if (Particle::particles[i] == Drawable::objects[j])
                    Drawable::objects.erase(Drawable::objects.begin() + j);
            for (int j = Collidable::objects.size() - 1; j >= 0; j--)
                if (Particle::particles[i] == Collidable::objects[j])
                    Collidable::objects.erase(Collidable::objects.begin() + j);

            delete Particle::particles[i];
            Particle::particles.erase(Particle::particles.begin() + i);
        }

    // std::cout << "Beginning Collidable Culling..." << std::endl;
    for (int i = Collidable::objects.size() - 1; i >= 0; i--)
        if (Collidable::objects[i] == NULL)
            Collidable::objects.erase(Collidable::objects.begin() + i);

    // std::cout << "Beginning Drawable Culling..." << std::endl;
    for (int i = Drawable::objects.size() - 1; i >= 0; i--)
        if (Drawable::objects[i] == NULL)
            Drawable::objects.erase(Drawable::objects.begin() + i);

    // std::cout << "Beginning Rigid Body Culling..." << std::endl;
    for (int i = Rigid_Body::objects.size() - 1; i >= 0; i--)
        if (Rigid_Body::objects[i] == NULL)
            Rigid_Body::objects.erase(Rigid_Body::objects.begin() + i);

    // std::cout << "Beginning Rigid Body Updates..." << std::endl;
    for (int i = 0; i < Rigid_Body::objects.size(); i++) {
        Rigid_Body::objects[i]->Update(dt);
    }

    Send();
    Receive();

    time_t now;
    time(&now);
    secondsSinceLastMessage = (now - secondsSinceLastMessageTick);

    if (secondsSinceLastMessage > 3 && teamWon == NO_TEAM)
    {
        std::cout << "Communication to server has timed out.\n";
        AppStateEvent::New_Event(APPSTATE_MASTER_LOBBY);
    }

    if (requestingGreeting)
        return;

    if (secondsToStart >= 0 && secondsToStart < 100)
    {
        if (now - secondsToStartLastTick >= 1)
        {
            time(&secondsToStartLastTick);
            secondsToStart--;
        }
    }

    if (teamWon != NO_TEAM)
    {
        if (secondsToEnd > 0)
        {
            if (now - secondsToEndLastTick >= 1)
            {
                time(&secondsToEndLastTick);
                secondsToEnd--;
            }
        }
        else
            AppStateEvent::New_Event(APPSTATE_MASTER_LOBBY);
    }

    // std::cout << "Beginning Player Updates..." << std::endl;
    player.Update(dt);

    // std::cout << "Beginning Collision Updates..." << std::endl;
    Collision_Manager::Get_Instance()->Update(dt);

    // std::cout << "Beginning Camera Updates..." << std::endl;
    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    viewport.x = player.pawn->x - viewport.w / 2.0;
    viewport.y = player.pawn->y - viewport.h / 2.0;
    Camera::getInstance()->Set_Viewport(viewport);
    // std::cout << "End of Updates..." << std::endl;
}

void AppStateGame::Draw() {
    TextureManager * textureManager = TextureManager::GetInstance();

    Camera * temp_camera = Camera::getInstance();
    Rect<double> temp_rect = temp_camera->Get_Viewport();

    if (requestingGreeting)
    {
        DrawText(20, temp_rect.h - 40, "Connecting...", textureManager->fonts.font_Impact_20, &WHITE);
        return;
    }

    Texture *backgroundTexture = textureManager->background_game2;
    glColor4f(1, 1, 1, 1);
    backgroundTexture->DrawAtRect(0, 0, 1024, 1024);

    Rect<double> viewport = Camera::getInstance()->Get_Viewport();
    for (int i = Drawable::objects.size() - 1; i >= 0; i--) {
        temp_camera->Map_To_Viewport(Drawable::objects[i]);
        Drawable::objects[i]->Draw();
        temp_camera->Map_To_World(Drawable::objects[i]);
    }

    player.Draw();

    if (secondsToStart >= 0 && secondsToStart < 100)
    {
        std::stringstream s;
        if (secondsToStart > 0)
            s << (int)secondsToStart;
        else
            s << "GO";
        std::string buf = s.str();
        int length = buf.length();
        Color color(1, 1, 1, .5);
        DrawText(temp_rect.w / 2.0 - length * 20, temp_rect.h / 2.0 - 40, buf.c_str(), textureManager->fonts.font_Impact_80, &color);
    }

    if (teamWon != NO_TEAM)
    {
        std::stringstream s;
        if (teamWon == RED_TEAM)
            s << "Red Team Won";
        else
            s << "Blue Team Won";
        std::string buf = s.str();
        int length = buf.length();
        Color color(1, 1, 1, .5);
        DrawText(temp_rect.w / 2.0 - length * 20, temp_rect.h / 2.0 - 40, buf.c_str(), textureManager->fonts.font_Impact_80, &color);
    }
}

void AppStateGame::Send() {
    if (teamWon != NO_TEAM)
        return;

    if (!requestingGreeting)
    {
        Parser parser;
        if (!parser.SerializeInput(player.inputs, NUMBER_OF_INPUTS))
            return;
        parser.End();

        network->SendData(parser.GetStream(), player.channel_id);
    }
    else
    {
        NetString string;
        string.WriteUChar(NCE_PLAYER_GREETING);
        string.WriteString(player.player_name);
        string.WriteUChar(NCE_END);
        network->SendData(&string, player.channel_id);
    }
}

void AppStateGame::Receive() {
    NetString netString;
    while (network->ReceiveData() != -1)
    {
        time(&secondsSinceLastMessageTick);
        secondsSinceLastMessage = 0;
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
            case NCE_NEW_CONNECTION:
            {
                unsigned char channelId = 0;
                netString.ReadUChar(channelId);

                if (player.channel_id == -1)
                {
                    player.channel_id = channelId;
                    network->Bind(channelId);
                }
                break;
            }

            case NCE_PLAYER_GREETING:
            {
                int temp;
                float temp2;
                netString.ReadInt(temp);
                player.player_id = temp;
                unsigned char tempc;
                netString.ReadUChar(tempc);
                player.team_id = (Team)temp;
                netString.ReadInt(temp);
                netString.ReadFloat(temp2);

                if (requestingGreeting)
                {
                    delete map;
                    map = new Map(temp, temp2);
                    map->Generate_Map();
                    Ship::Initialize_Ships(map->max_players_per_team * 2);

                    if (player.pawn == NULL)
                    {
                        int pawn_index = Ship::Add_Ship(player.team_id, INTERCEPTOR, 100, 100, 5.0);
                        if (pawn_index >= 0)
                            player.pawn = Ship::ships[pawn_index];
                        else
                        {
                            std::cerr << "Error initializing ship\n";
                            AppStateEvent::New_Event(APPSTATE_MENU);
                        }

                        players[player.player_id] = player.pawn;
                    }

                    std::cout << "I am player: " << (int)player.player_id << "; team: " << player.team_id << '\n';
                    std::cout << "Map: " << temp << "; Scale: " << temp2 << '\n';
                    player.info_feed.Player_Joined(player.player_name);
                    requestingGreeting = false;
                }

                break;
            }

            case NCE_NEW_PLAYER:
            {
                int temp, player_id, i;
                netString.ReadInt(player_id);
                unsigned char tempc;
                netString.ReadUChar(tempc);
                Team team_id = (Team)tempc;

                std::string player_name;
                netString.ReadString(player_name);

                if (players[player_id] != NULL)
                {
                    players[player_id]->team_id = team_id;
                    players[player_id]->player_name = player_name;
                }

                break;
            }

            case NCE_LOOKUP_PLAYER:
            {
                int temp, i, player_id;
                netString.ReadInt(player_id);
                unsigned char tempc;
                netString.ReadUChar(tempc);
                Team team_id = (Team)tempc;

                std::string player_name;
                netString.ReadString(player_name);
                
                if (players[player_id] != NULL)
                {
                    players[player_id]->team_id = team_id;
                    players[player_id]->player_name = player_name;
                }

                std::cout << "New Player Joined: " << player_name << "; ID: " << player_id << "; team: " << team_id << '\n';
                player.info_feed.Player_Joined(player_name);
                break;
            }

            case NCE_PLAYER:
            {
                unsigned char playerId;
                netString.ReadUChar(playerId);
                Ship *ship;

                // New Player?
                if (players[playerId] == NULL)
                {
                    int pawn_index = Ship::Add_Ship(NO_TEAM, INTERCEPTOR, 100, 100, 5.0);
                    if (pawn_index >= 0)
                        players[playerId] = Ship::ships[pawn_index];
                    else
                    {
                        std::cerr << "Error initializing ship\n";
                        AppStateEvent::New_Event(APPSTATE_MENU);
                    }

                    NetString query;
                    query.WriteUChar(NCE_LOOKUP_PLAYER);
                    query.WriteUChar(playerId);
                    query.WriteUChar(NCE_END);
                    network->SendData(&query, player.channel_id);
                    std::cout << "Requesting lookup on player: " << (int)playerId << '\n';
                }

                ship = players[playerId];
                ship->Deserialize(&netString);
                ship->Sync();

                // Could change this into a loop? eh..
                bool firedInputs[5];
                netString.ReadBool(firedInputs[0]);
                if (firedInputs[0])
                    ship->Fire(ENERGY_TYPE);
                netString.ReadBool(firedInputs[1]);
                if (firedInputs[1])
                    ship->Fire(BALLISTIC_TYPE);
                netString.ReadBool(firedInputs[2]);
                if (firedInputs[2])
                    ship->Fire(PROPELLED_TYPE);
                netString.ReadBool(firedInputs[3]);
                if (firedInputs[3])
                    ship->Fire(BOMB_TYPE);
                netString.ReadBool(firedInputs[4]);
                if (firedInputs[4])
                    ship->Fire(POWERUP_TYPE);
                break;
            }

            case NCE_REMOVE_PLAYER:
            {
                unsigned char playerId;
                netString.ReadUChar(playerId);
                if (players[playerId] != NULL && players[playerId] != player.pawn)
                {
                    player.info_feed.Player_Disconnected(players[playerId]->player_name);
                    Ship::Remove_Ship(players[playerId]);
                    players[playerId] = NULL;
                
                    std::cout << "Player Removed: " << (int)playerId << '\n';
                }
                break;
            }

            case NCE_LOBBY_STATE_SYNC:
            {
                GameServerEnums serverState;
                unsigned char temp;
                if (!netString.ReadUChar(temp))
                    break;
                serverState = (GameServerEnums)temp;

                switch (serverState)
                {
                    case GSE_GAME_COUNTDOWN:
                    {
                        int tempI;
                        unsigned char tempC;
                        netString.ReadUChar(tempC);
                        secondsToStart = tempC;
                        netString.ReadInt(tempI);
                        secondsToStartLastTick = tempI;
                        break;
                    }

                    case GSE_GAME_ENDED:
                    {
                        unsigned char tempC;
                        netString.ReadUChar(tempC);
                        teamWon = (Team)tempC;
                        time(&secondsToEndLastTick);
                        secondsToEnd = 5;
                        break;
                    }
                }
            }

            case NCE_END:

                break;

            default:
                reading = false;
        }
    }
}

void AppStateGame::Cleanup() {
    if (teamWon == NO_TEAM)
    {
        // no team won so the player must be exiting.
        NetString string;
        string.WriteUChar(NCE_PLAYER_EXITED);
        string.WriteUChar(NCE_END);
        network->SendData(&string, player.channel_id);
        std::cout << "Exiting\n";
    }

    for (int i = 0; i < MaximumClients; ++i)
    {
        if (players[i] != NULL && players[i] != player.pawn)
            Ship::Remove_Ship(players[i]);
    }

    network->Close();
    delete network;
}

AppStateBase * AppStateGame::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateGame();
    return instance;
}

void AppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = true;               break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = true;              break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = true;            break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = true;           break;
    case SDLK_1:        player.inputs[FIRE_ENERGY] = true;             break;
    case SDLK_2:        player.inputs[FIRE_BALLISTIC] = true;          break;
    case SDLK_3:        player.inputs[FIRE_PROPELLED] = true;          break;
    case SDLK_4:        player.inputs[FIRE_MINE] = true;               break;
    case SDLK_5:        player.inputs[FIRE_POWERUP] = true;            break;
    case SDLK_d:        player.pawn->health = 0.25 * player.pawn->max_health;           break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:
        break;
    }
}

void AppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     player.inputs[TURN_LEFT] = false;               break;
    case SDLK_RIGHT:    player.inputs[TURN_RIGHT] = false;              break;
    case SDLK_UP:       player.inputs[MOVE_FORWARD] = false;            break;
    case SDLK_DOWN:     player.inputs[MOVE_BACKWARD] = false;           break;
    case SDLK_1:        player.inputs[FIRE_ENERGY] = false;             break;
    case SDLK_2:        player.inputs[FIRE_BALLISTIC] = false;          break;
    case SDLK_3:        player.inputs[FIRE_PROPELLED] = false;          break;
    case SDLK_4:        player.inputs[FIRE_MINE] = false;               break;
    case SDLK_5:        player.inputs[FIRE_POWERUP] = false;            break;
    default:
        break;
    }
}