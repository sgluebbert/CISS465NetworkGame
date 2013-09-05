#ifndef APPSTATEBASE_H
#define APPSTATEBASE_H

#include <SDL.h>

#include "EventHandler.h"
 
class AppStateBase : public EventHandler {
    protected:
        bool is_running;
        
    public:
        AppStateBase();
 
        virtual void Initialize() = 0;
        virtual void Events(SDL_Event *) = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;
        virtual void Cleanup() = 0;
};

AppStateBase::AppStateBase() {
}

#endif
