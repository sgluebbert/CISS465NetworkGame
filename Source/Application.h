#ifndef APPLICATION_H
#define APPLICATION_H



#include <SDL.h>
#include <iostream>

#include "AppStateManager.h"
#include "EventHandler.h"
#include "Timer.h"
#include "System.h"



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



#endif
