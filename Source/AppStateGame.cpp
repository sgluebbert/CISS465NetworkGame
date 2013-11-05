#include "AppStateGame.h"



AppStateBase * AppStateGame::instance = NULL;



AppStateGame::AppStateGame() {
}

void AppStateGame::Initialize() {
    background_texture = surface_manager->background_game;
    background_rect = Camera::getInstance()->Get_Viewport();

    if (network->InitClient() == -1)
        AppStateEvent::New_Event(APPSTATE_MENU);
    
    // sound_manager->Load_Music("./Sound/Music/Battle.ogg");
    // sound_manager->Play_Music();
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
    /*network->SendData(inputs, client_channel, 4);

    while (network->ReceiveData()) {
        NetString netString(network->GetData(), network->GetDataLength());

        bool reading = true;

        while (reading)
        {

	        NetworkChunkEnums type;
            unsigned char temp;
            if (!netString.ReadUChar(temp))
                break;

            type = (NetworkChunkEnums)temp;

            unsigned char team = 0;
            Ship * ship = NULL;

            switch (type) {
                case NCE_NEW_PLAYER:
                    netString.ReadUChar(team);

                    if (player == NULL)
                    {
                        client_channel = team;
                        std::cout << "New Channel: " << (int)team << '\n';
                        network->Bind(team);

                        player = new Ship(team);
                        player->SetTexture(surface_manager->ship, 64, 64);
                        ships.push_back(player);
                    }
                    player_chat.Player_Joined(team);
                    break;

                case NCE_PLAYER:
                    {
                        netString.ReadUChar(team);
                        
		                for (int i = 0; i < ships.size(); i++)
		                {
		                	if (ships[i] == NULL)
		                		continue;
		                	if (ships[i]->team == team)
		                	{
		                		ship = ships[i];
		                		break;
		                	}
		                }

			        	if (ship == NULL) // New player
			        	{
			        		ship = new Ship(team);
			        		ships.push_back(ship);
		                    ship->SetTexture(surface_manager->ship, 64, 64);
                    		player_chat.Player_Joined(team);
			        	}

                        netString.ReadFloat(ship->x);
                        netString.ReadFloat(ship->y);
                        netString.ReadFloat(ship->angle);
                        netString.ReadFloat(ship->health);
                        bool shot;
                        netString.ReadBool(shot);

	                    if (shot)
	                    {
	                    	int offset_x = 20 * TRIG_TABLE[int(ship->angle / 5.0)][1];
            				int offset_y = -20 * TRIG_TABLE[int(ship->angle / 5.0)][0];
	                    	bullet_list->AddBullet(ship->team, ship->x + offset_x, ship->y + offset_y, ship->velocity + 120, ship->angle);
	                    }
		                break;
		            }

	            case NCE_REMOVE_PLAYER:
                    netString.ReadUChar(team);
                    player_chat.Player_Disconnected(team);
	                for (int i = 0; i < ships.size(); i++)
	                {
	                	if (ships[i] == NULL)
	                		continue;
	                	if (ships[i]->team == team)
	                	{
	                		delete ships[i];
	                		ships[i] = NULL;
	                		break;
	                	}
	                }

	                if (player != NULL &&  player->team == team)
	                {
	                	Cleanup();
	                	AppStateEvent::New_Event(APPSTATE_MENU);
	                	return;
	                }

	            case NCE_END:
	            	reading = false;
	            	break;

                default:
                    reading = false;
                    break;
	        }
	    }
    }

    if (ships.size() > 0 && ships.front() == NULL) {
    	ships.pop_front();
    }

    if (ships.size() > 0 && ships.back() == NULL) {
    	ships.pop_back();
    }

    for (int i = 0; i < ships.size(); i++)
    {
    	if (ships[i] == NULL)
    		continue;
    	ships[i]->Update();
    }
    
    bullet_list->Update();

    player_health.Notify(player);
    player_radar.Notify(ships);
    player_radar.Notify(player);

    if (player != NULL)   {
	    SDL_Rect viewport = Camera::getInstance()->Get_Viewport();
        viewport.x = player->x + player->width / 2.0 - viewport.w / 2.0;
        viewport.y = player->y + player->height / 2.0 - viewport.h / 2.0;
        Camera::getInstance()->Set_Viewport(viewport);
    }


    background_rect = Camera::getInstance()->Get_Viewport();*/
}

void AppStateGame::Draw() {
    Camera * temp_camera = Camera::getInstance();
}

void AppStateGame::Cleanup() {
    sound_manager->Stop_Music();
}

AppStateBase * AppStateGame::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateGame();
    return instance;
}

void AppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:
        break;
    }
}

void AppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}