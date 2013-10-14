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



enum NetworkType { END, NEW_PLAYER, PLAYER, REMOVE_PLAYER, BULLET, COLLISION };
 


class AppStateGame : public AppStateBase {
private:
        static AppStateBase * instance;
        AppStateGame();
        
        SDL_Surface * background_surf;
        SDL_Rect background_rect;
        
        static const char * MUSIC_FILENAME;
        
        Ship * player;
        std::deque<Ship * > ships;

        Chat_Window player_chat;
        Health_Bar player_health;
        Radar player_radar;

	    Uint8 inputs[4];
	
	    IPaddress server_address;
	    char * host_address;
	    Uint16 server_port;
	    Uint16 host_port;
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
    background_surf = surface_manager->background_game;
    background_rect = Camera::getInstance()->Get_Viewport();
    
    host_address = NULL;
    client_channel = -1;
    host_port = 8080;
    server_port = 8080;
	    
	for (int i = 0; i < 4; i++)
	    inputs[i] = 0;
 
	/* Resolve server name  */
	if (SDLNet_ResolveHost(&server_address, host_address, server_port) == -1) {
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "", server_port, SDLNet_GetError());
		AppStateEvent::New_Event(APPSTATE_MENU);
	}
    
    //p->address.host = server_address.host;	/* Set the destination host */
    //p->address.port = server_address.port;	/* And destination port */
    //p->data = "Hello!";
    //p->len = 7;
    //SDLNet_UDP_Send(sd, -1, p); /* This sets the p->channel */
    
    //Hangs the program until it has communicated with the server
    //To get id for player array
    //while (!SDLNet_UDP_Recv(sd, p)) {}

   	player = NULL;
    
    send->address.host = server_address.host;	/* Set the destination host */
    send->address.port = server_address.port;	/* And destination port */
    send->len = 4;
    
    sound_manager->Load_Music(MUSIC_FILENAME);
    sound_manager->Play_Music();
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
	// if (player == NULL || (player != NULL && player->team == 0))
	{
	    send->data = inputs;
	    SDLNet_UDP_Send(socket, client_channel, send); /* This sets the p->channel */
	}
    
    Bullet_List *bullet_list = Bullet_List::getInstance();

    while (SDLNet_UDP_Recv(socket, recieve)) {
        // std::cout << recieve->len << '\n';
        unsigned char *buffer = recieve->data;
        // for (int i = 0; i < recieve->len; i ++)
        //     std::cout << (int)buffer[i] << ' ';
        // std::cout << '\n';

        bool reading = true;
        unsigned int index = 0;
        unsigned int packets_read = 0;

        while (reading)
        {
	        NetworkType type = (NetworkType)buffer[index++];
	        int team;

	        Ship * ship = NULL;

	        switch (type) {
	            case NEW_PLAYER:
                    team = buffer[index++];
	                if (player == NULL)
	                {
	        			client_channel = team;
	        			std::cout << "New Channel: " << team << '\n';
	        			SDLNet_UDP_Bind(socket, team, &recieve->address);
	    				// send->channel = client_channel;

	                    player = new Ship(team);
	                    player->SetSurface(surface_manager->ship, 64, 64);
		        		ships.push_back(player);
	                    player->max_velocity = 50;
	                    player->acceleration = 8;
	                    player->deceleration = 6;
	                    player->turn_rate = 30;
	                }
                    player_chat.Player_Joined(team);
	                break;

	            case PLAYER:
	            	{
	                    team = buffer[index++];

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
		                    ship->SetSurface(surface_manager->ship, 64, 64);
		                    ship->max_velocity = 50;
		                    ship->acceleration = 8;
		                    ship->deceleration = 6;
		                    ship->turn_rate = 30;
                    		player_chat.Player_Joined(team);
			        	}

			        	ship->x = read_float(buffer + index); index += 4;
			        	ship->y = read_float(buffer + index); index += 4;
	                    ship->angle = read_float(buffer + index); index += 4;
	                    ship->health = read_float(buffer + index); index += 4;
	                    bool shot = buffer[index++];
	                    if (shot)
	                    {
	                    	int offset_x = 20 * TRIG_TABLE[int(ship->angle / 5.0)][1];
            				int offset_y = -20 * TRIG_TABLE[int(ship->angle / 5.0)][0];
	                    	bullet_list->AddBullet(ship->team, ship->x + offset_x, ship->y + offset_y, ship->velocity + 120, ship->angle);
	                    	std::cout << "Shot\n";
	                    }
		                break;
		            }

	            case REMOVE_PLAYER:
                    team = buffer[index++];
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

                // case BULLET:
                //     team = SDLNet_Read32(buffer + index); index += 4;
                //     { // {} used to avoid switch scope complaining for now.
                //         float x = read_float(buffer + index); index += 4;
                //         float y = read_float(buffer + index); index += 4;
                //         // bullet_list.AddBullet(x, y, velocity, angle);
                //     }
                //     break;

                case COLLISION:
                    { // {} used to avoid switch scope complaining for now.
                        int type = SDLNet_Read32(buffer + index); index += 4;
                        float x = read_float(buffer + index); index += 4;
                        float y = read_float(buffer + index); index += 4;
                    }
                    break;

	            case END:
	            	reading = false;
	            	break;

                default:
                    reading = false;
                    break;
	        }

            packets_read++;

	    }

        if (packets_read == 0 && player != NULL) {
            player->turn_left = inputs[0];
            player->turn_right = inputs[1];
            player->move_forward = inputs[2];
            player->shoot = inputs[3];
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
    SDL_BlitSurface(background_surf, &background_rect, WINDOW, NULL);

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
