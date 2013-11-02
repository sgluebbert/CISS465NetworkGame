#ifndef APPSTATEGAME_H
#define APPSTATEGAME_H



#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <deque>

#include "AppStateBase.h"
#include "Bullet_List.h"
#include "Camera.h"
#include "Chat_Window.h"
#include "Health_Bar.h"
#include "Radar.h"
#include "System.h"
#include "Networking/Parser.h"

Network* NetworkFactory::instance = NULL;
NetworkType NetworkFactory::networkType = UNDEFINED;
Network *network = NetworkFactory::getInstance();

class AppStateGame : public AppStateBase {
private:
        static AppStateBase * instance;
        AppStateGame();
        
        Texture * background_texture;
        SDL_Rect background_rect;
        
        static const char * MUSIC_FILENAME;
        
        Ship * player;
        std::deque<Ship * > ships;

        Chat_Window player_chat;
        Health_Bar player_health;
        Radar player_radar;

	    Uint8 inputs[4];

	    int client_channel;
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



AppStateBase * AppStateGame::instance = NULL;

const char * AppStateGame::MUSIC_FILENAME = "./Sound/Music/Battle.ogg";



AppStateGame::AppStateGame() {
}

void AppStateGame::Initialize() {
    background_texture = surface_manager->background_game;
    background_rect = Camera::getInstance()->Get_Viewport();
    
    client_channel = -1;
   
	for (int i = 0; i < 4; i++)
	    inputs[i] = 0;

    if (network->InitClient() == -1)
    {
        AppStateEvent::New_Event(APPSTATE_MENU);
    }

   	player = NULL;
    
    sound_manager->Load_Music(MUSIC_FILENAME);
    sound_manager->Play_Music();
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
    
    network->SendData(inputs, client_channel, 4);

    Bullet_List *bullet_list = Bullet_List::getInstance();

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


    background_rect = Camera::getInstance()->Get_Viewport();
}

void AppStateGame::Draw() {
    Camera * temp_camera = Camera::getInstance();

    temp_camera->Draw_Ships(ships);
    temp_camera->Draw_Bullets(Bullet_List::getInstance());

    player_chat.Draw();
    player_health.Draw();
    player_radar.Draw();
}

void AppStateGame::Cleanup() {
    sound_manager->Stop_Music();

	for (int i = 0; i < ships.size(); i++)
    {
    	if (ships[i] == NULL)
    		continue;
    	delete ships[i];
    	ships[i] = NULL;
    }
}

AppStateBase * AppStateGame::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateGame();
    return instance;
}

void AppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     inputs[0] = 1;                              break;
    case SDLK_RIGHT:    inputs[1] = 1;                              break;
    case SDLK_UP:       inputs[2] = 1;                              break;
    case SDLK_SPACE:    inputs[3] = 1;                              break;
    case SDLK_j:        player_chat.Player_Joined(5);               break;
    case SDLK_l:        player_chat.Player_Disconnected(5);         break;
    case SDLK_d:        player_chat.Player_Died(5);                 break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:
        break;
    }
}

void AppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     inputs[0] = 0;                              break;
    case SDLK_RIGHT:    inputs[1] = 0;                              break;
    case SDLK_UP:       inputs[2] = 0;                              break;
    case SDLK_SPACE:    inputs[3] = 0;                              break;
    default:
        break;
    }
}



#endif
