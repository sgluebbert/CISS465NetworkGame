#ifndef APPSTATEGAME_H
#define APPSTATEGAME_H

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "AppStateBase.h"
#include "Bullet.h"
#include "Camera.h"
#include "Entity.h"
#include "Player.h"
#include "Sound.h"
#include "SurfaceManager.h"
#include "System.h"
 
class AppStateGame : public AppStateBase {
private:
        static AppStateBase * instance;
        AppStateGame();
        
        SDL_Surface * background_surf;
        SDL_Rect background_rect;
        
        static const char * BACKGROUND_FILENAME;
        static const char * MUSIC_FILENAME;
        
        Entity *player;
		
	    UDPsocket sd;	/* Socket Descriptor */
	    UDPpacket * recieve;	/* Pointer to packet memory */
	    UDPpacket * send;	/* Pointer to packet memory */
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

const char * AppStateGame::BACKGROUND_FILENAME = "./Art/Background.bmp";
const char * AppStateGame::MUSIC_FILENAME = "./Sound/Music/Battle.ogg";

AppStateGame::AppStateGame() {
}

void AppStateGame::Initialize() {
    SurfaceManager * surfaceManager = SurfaceManager::getInstance();
    background_surf = background_surf = surfaceManager->background_game01;
    //background_rect;
    
    host_address = NULL;
    client_channel = -1;
    host_port = 8080;
    server_port = 8080;
	    
	for (int i = 0; i < 4; i++)
	    inputs[i] = 0;
    
    /* Initialize SDL_net */
	if (SDLNet_Init() < 0) {
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		AppStateEvent::New_Event(APPSTATE_MENU);
	}
 
	/* Open a socket on random port */
	if (!(sd = SDLNet_UDP_Open(0))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		AppStateEvent::New_Event(APPSTATE_MENU);
	}
 
	/* Resolve server name  */
	if (SDLNet_ResolveHost(&server_address, host_address, server_port) == -1) {
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", "", server_port, SDLNet_GetError());
		AppStateEvent::New_Event(APPSTATE_MENU);
	}
 
	/* Allocate memory for the packet */
	if (!(send = SDLNet_AllocPacket(512))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		AppStateEvent::New_Event(APPSTATE_MENU);
    }
 
	/* Allocate memory for the packet */
	if (!(recieve = SDLNet_AllocPacket(512))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
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
    
    send->address.host = server_address.host;	/* Set the destination host */
    send->address.port = server_address.port;	/* And destination port */
    send->len = 4;
    
    MUSIC_STREAM.load(MUSIC_FILENAME);
    MUSIC_STREAM.play();

    player = NULL;
    // player->SetSurface(surfaceManager->ship01, 64, 64);
    // player->max_velocity = 50;
    // player->acceleration = 8;
    // player->deceleration = 6;
    // player->turn_rate = 30;
}

void AppStateGame::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateGame::Update() {
    send->data = inputs;
    SDLNet_UDP_Send(sd, client_channel, send); /* This sets the p->channel */
    
    while (SDLNet_UDP_Recv(sd, recieve)) {
        std::cout << (char *)recieve->data << std::endl;

        std::stringstream s;
        s << recieve->data;
        char type;
        s >> type;
        int team;

        SurfaceManager *surfaceManager;
        switch (type)
        {
            case 'N':
                s >> team;
                surfaceManager = SurfaceManager::getInstance();
                if (player == NULL)
                {
                    player = new Entity(team);
                    player->SetSurface(surfaceManager->ship01, 64, 64);
                    player->max_velocity = 50;
                    player->acceleration = 8;
                    player->deceleration = 6;
                    player->turn_rate = 30;
                }
                break;
            case 'P':
                s >> team;

                // get entity based on team
                // if team is not found, add the player

                Entity *entity = player;

                s >> entity->x >> entity->y >> entity->angle;
                break;
        }
    }

    inputs[3] = 0;
    
    //Bullet_List::getInstance()->Update();
    // player->Update();
	SDL_Rect viewport = Camera::getInstance()->Get_Viewport();
    if (player != NULL)
    {
        viewport.x = player->x - viewport.w / 2.0;
        viewport.y = player->y - viewport.h / 2.0;
    }
    Camera::getInstance()->Set_Viewport(viewport);

}

void AppStateGame::Draw() {
	Surface::Blit(WINDOW, background_surf, 0, 0);
    // SDL_BlitSurface(WINDOW, &WINDOW_BOUNDING_BOX, background_surf, &background_rect);
    
    // SDL_Rect rect = {200, 150, 400, 300};
    // Surface::DrawRect(WINDOW, rect, CYAN);
    if (player != NULL)
        player->Draw();
    Bullet_List::getInstance()->Draw();
    // camera.Draw();//<------This should replace the above two lines at some 
}

void AppStateGame::Cleanup() {
    MUSIC_STREAM.stop();
	SDLNet_FreePacket(recieve);
	// SDLNet_FreePacket(send);
	SDLNet_Quit();
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
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:
        if (player != NULL)
        {
            std::cout << "Pos: [" << player->x << ", " << player->y << "]\n";
            std::cout << "Size: [" << player->width << ", " << player->height << "]\n";
            std::cout << "Speed: [" << player->dx << ", " << player->dy << "]\n";
            std::cout << "Velocity: [" << player->velocity << "/" << player->max_velocity << "]\n";
            std::cout << "Throttle: [" << player->throttle << "]\n";
            std::cout << "Angle: [" << player->angle << "]\n";
        }
        break;
    }
}

void AppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_LEFT:     inputs[0] = 0;                              break;
    case SDLK_RIGHT:    inputs[1] = 0;                              break;
    case SDLK_UP:       inputs[2] = 0;                              break;
        break;
    default:
        break;
    }
}

#endif
