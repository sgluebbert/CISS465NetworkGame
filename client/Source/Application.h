#ifndef APPLICATION_H
#define APPLICATION_H



#include <SDL.h>
#include <iostream>

#include "AppStateManager.h"
#include "EventHandler.h"
#include "Timer.h"
#include "Surface_Manager.h"

//Inludes for Compiling/Debugging Purposes



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
};

Application::Application() {
    WINDOW = NULL;
    WINDOW_BOUNDING_BOX = DEFAULT_WINDOW_BOUNDING_BOX;
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
    if (!Initialize_System())
        return false;
    
    AppStateManager::Initialize();
    
    return true;
}

void Application::Events(SDL_Event * Event) {
    //Sends Events to appropriate functions
    EventHandler::OnEvent(Event);
    AppStateManager::Events(Event);
}

void Application::Draw() {
    Clear_Window();
	
    AppStateManager::Draw();

    SDL_Flip(WINDOW);
}

void Application::Update() {
    Application_Event * appFlag = Application_Event::Poll_Event();
    
    if (appFlag != NULL)
        is_running = false;

    AppStateManager::Update();

    Timer::Frame_Control.Update();

    char Buffer[255];
    sprintf(Buffer, "FPS: %d", Timer::Frame_Control.Get_FPS());
    SDL_WM_SetCaption(WINDOW_TITLE, Buffer);
}

void Application::Cleanup() {
    Cleanup_System();

    AppStateManager::Cleanup();

    SDL_FreeSurface(WINDOW);
    SDL_Quit();
}

void Application::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    EventHandler::OnKeyDown(sym, mod, unicode);
}

void Application::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    EventHandler::OnKeyUp(sym, mod, unicode);
}

void Application::OnMinimize() {
}

void Application::OnRestore() {
}

void Application::OnResize(int w, int h) {
    WINDOW_BOUNDING_BOX.w = w;
    WINDOW_BOUNDING_BOX.h = h;
    Reset_Window(WINDOW_BOUNDING_BOX);
}

void Application::OnExpose() {
}

void Application::OnExit() {
    is_running = false;
}

void Application::OnUser(Uint8 type, int code, void* data1, void* data2) {
}

#endif
