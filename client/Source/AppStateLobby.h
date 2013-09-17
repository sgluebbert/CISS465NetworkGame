#ifndef APPSTATELOBBY_H
#define APPSTATELOBBY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AppStateBase.h"
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
#include "Sound.h"
#include "SurfaceManager.h"
#include "System.h"
 
class AppStateLobby : public AppStateBase {
private:
        static AppStateBase * instance;
        
        SDL_Surface * background_surf;
        SDL_Rect background_rect;
		
		char * text_buffer;
		
	    UDPsocket sd;	/* Socket Descriptor */
	    UDPpacket *p;	/* Pointer to packet memory */
	
	    IPaddress server_address;
	    char * host_address;
	    Uint16 server_port;
        
        static const int TEXT_BUFFER_SIZE;
		
        static const char * BACKGROUND_FILENAME;
        static const char * MUSIC_FILENAME;
public:
		AppStateLobby();
 
        void Initialize();
        void Events(SDL_Event *);
        void Update();
        void Draw();
        void Cleanup();
        
        static AppStateBase * GetInstance();
 
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
};



AppStateBase * AppStateLobby::instance = NULL;

const int AppStateLobby::TEXT_BUFFER_SIZE = 255;

const char * AppStateLobby::BACKGROUND_FILENAME = "./Art/Background.bmp";
const char * AppStateLobby::MUSIC_FILENAME = "./Sound/Music/Battle.ogg";

AppStateLobby::AppStateLobby() {
}

void AppStateLobby::Initialize() { 
    //SurfaceManager * surfaceManager = SurfaceManager::getInstance();
    background_surf = NULL;//surfaceManager->background_main_menu01;
    //background_rect;
    
    host_address = NULL;
    server_port = 2000;
    
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
	if (!(p = SDLNet_AllocPacket(512))) {
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		AppStateEvent::New_Event(APPSTATE_MENU);
	}
	
	text_buffer = new char[TEXT_BUFFER_SIZE];
}

void AppStateLobby::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateLobby::Update() {
		/*This will need to be moved to Key States to keep track of key inputs
	      rather than hanging on this section before doing anything else.*/
        printf("Fill the buffer\n>");
        scanf("%s", (char *)p->data);
        
        p->address.host = server_address.host;	/* Set the destination host */
        p->address.port = server_address.port;	/* And destination port */
        
        p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(sd, -1, p); /* This sets the p->channel */
        
        /* Quit if packet contains "quit" */
        if (!strcmp((char *)p->data, "quit"))
            AppStateEvent::New_Event(APPSTATE_MENU);
}

void AppStateLobby::Draw() {
}

void AppStateLobby::Cleanup() {
	delete[] text_buffer;
	
	SDLNet_FreePacket(p);
	SDLNet_Quit();
}

AppStateBase * AppStateLobby::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateLobby();
    return instance;
}

void AppStateLobby::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
	//ASCII Keys need to be registered and the mod will need to be used to
	//allow for use of capitals and such.
    //case SDLK_RETURN:   p->data = (Uint8 *)text_buffer;				break;
    case SDLK_ESCAPE:   AppStateEvent::New_Event(APPSTATE_NONE);    break;
    case SDLK_TAB:      AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:            break;
    }
}

void AppStateLobby::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}

#endif
