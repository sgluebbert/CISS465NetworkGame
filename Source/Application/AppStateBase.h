#ifndef APPSTATEBASE_H
#define APPSTATEBASE_H



#include <SDL.h>

#include "AppStateEvent.h"
#include "EventHandler.h"
 


class AppStateBase : public EventHandler {
public:
    AppStateBase();
    virtual ~AppStateBase();
 
    virtual void Initialize() = 0;
    virtual void Events(SDL_Event *) = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Cleanup() = 0;
};



#endif
