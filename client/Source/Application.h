#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL.h>

#include "Animation.h"
#include "AppStateManager.h"
#include "Camera.h"
#include "Entity.h"
#include "EventHandler.h"
#include "FPS.h"
#include "Menu.h"
#include "Sound.h"
#include "System.h"

class Application : public EventHandler {
public:
    Application();
    
    virtual bool Execute();
    virtual bool Initialize();
    virtual void Events(SDL_Event *);
    virtual void Draw();
    virtual void Update();
    virtual void Cleanup();
    
    void OnExit();
    
protected:
    bool is_running;
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
    
    buildTrigTable();
    buildKeyArray();
    
    AppStateManager::Initialize();
    
    return true;
}

void Application::Events(SDL_Event * Event) {
    EventHandler::OnEvent(Event);
    AppStateManager::Events(Event);
}

void Application::Draw() {
    AppStateManager::Draw();

    SDL_Flip(WINDOW);
}

void Application::Update() {
    AppStateManager::Update();

    FPS::FPSControl.Update();

    char Buffer[255];
    sprintf(Buffer, "FPS: %d", FPS::FPSControl.GetFPS());
    SDL_WM_SetCaption(WINDOW_TITLE, Buffer);
}

void Application::Cleanup() {
    AppStateManager::Cleanup();

    SDL_FreeSurface(WINDOW);
    SDL_Quit();
}

void Application::OnExit() {
    is_running = false;
}

#endif
