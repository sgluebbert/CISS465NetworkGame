#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL.h>
#include <iostream>

#include "Animation.h"
//#include "AppStateManager.h"
#include "Camera.h"
#include "Entity.h"
#include "EventHandler.h"
#include "FPS.h"
#include "Menu.h"
#include "Player.h"

class Application : public EventHandler {
public:
    Application();
    
    bool Execute();
    
    bool Initialize();
    void Events(SDL_Event *);
    void Draw();
    void Update();
    void Cleanup();
 
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
        
    void OnMinimize();
    void OnRestore();
    void OnResize(int w,int h);
    void OnExpose();
    void OnExit();
    void OnUser(Uint8 type, int code, void * data1, void * data2);
    
protected:
    bool is_running;
    Entity pawn;
};

Application::Application() {
    WINDOW = NULL;
    is_running = true;
}

bool Application::Execute() {
    if (!Initialize())
        return -1;

    SDL_Event Event;
        
    while (is_running) {
        while(SDL_PollEvent(&Event))
            Events(&Event);
        Draw();
        Update();
    }
    
    Cleanup();
    return 0;
}

bool Application::Initialize() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;
        
	SDL_WM_SetCaption(WINDOW_TITLE, NULL);
	//SDL_WM_SetIcon(WINDOW_ICON_FILEPATH, NULL);

    if((WINDOW = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
        return false;

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    
    Build_Trig_Table();
    Build_Key_Array();
    
    pawn.LoadSurface("./Art/Untitled.bmp", 40, 40);
    pawn.max_velocity = 10;
    pawn.acceleration = 2;
    pawn.deceleration = 2;
    
    //AppStateManager::Initialize();
    
    return true;
}

void Application::Events(SDL_Event * Event) {
    //Sends Events to appropriate functions
    EventHandler::OnEvent(Event);
    //AppStateManager::Events(Event);
}

void Application::Draw() {
    Clear_Window();
    
    pawn.Draw();
    //AppStateManager::Draw();

    SDL_Flip(WINDOW);
}

void Application::Update() {
    if (pawn.throttle < 1.0)
        pawn.throttle += 0.001;
    pawn.Update();
    //AppStateManager::Update();

    FPS::FPSControl.Update();

    char Buffer[255];
    sprintf(Buffer, "FPS: %d", FPS::FPSControl.GetFPS());
    SDL_WM_SetCaption(WINDOW_TITLE, Buffer);
}

void Application::Cleanup() {
    //AppStateManager::Cleanup();

    SDL_FreeSurface(WINDOW);
    SDL_Quit();
}

void Application::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    //Updates the Key Pressed Table
    EventHandler::OnKeyDown(sym, mod, unicode);
    
    //Prints debugging on any key press
    switch(sym) {
    default:
        std::cout << "Pos: [" << pawn.x << ", " << pawn.y << "]\n";
        std::cout << "Size: [" << pawn.width << ", " << pawn.height << "]\n";
        std::cout << "Speed: [" << pawn.dx << ", " << pawn.dy << "]\n";
        std::cout << "Velocity: [" << pawn.velocity << "/" << pawn.max_velocity << "]\n";
        std::cout << "Throttle: [" << pawn.throttle << "]\n";
        std::cout << "Angle: [" << pawn.angle << "]\n";
        break;
    }
}

void Application::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    //Updates the Key Pressed Table
    EventHandler::OnKeyUp(sym, mod, unicode);
    
    switch(sym) {
    default:
        break;
    }
}

void Application::OnMinimize() {
}

void Application::OnRestore() {
}

void Application::OnResize(int w, int h) {
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    WINDOW = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
}

void Application::OnExpose() {
}

void Application::OnExit() {
    is_running = false;
}

void Application::OnUser(Uint8 type, int code, void* data1, void* data2) {
}

#endif
