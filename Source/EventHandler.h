#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL.h>

#include "System.h"

class EventHandler {
public:
        EventHandler();
        ~EventHandler();
 
        virtual void OnEvent(SDL_Event * Event);
 
        virtual void OnInputFocus();
        virtual void OnInputBlur();
 
        virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
 
        virtual void OnMouseFocus();
        virtual void OnMouseBlur();
        virtual void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
        virtual void OnMouseWheel(bool Up, bool Down);
        virtual void OnLButtonDown(int mX, int mY);
        virtual void OnLButtonUp(int mX, int mY);
        virtual void OnRButtonDown(int mX, int mY);
        virtual void OnRButtonUp(int mX, int mY);
        virtual void OnMButtonDown(int mX, int mY);
        virtual void OnMButtonUp(int mX, int mY);
        
        virtual void OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
        virtual void OnJoyButtonDown(Uint8 which, Uint8 button);
        virtual void OnJoyButtonUp(Uint8 which, Uint8 button);
        virtual void OnJoyHat(Uint8 which, Uint8 hat, Uint8 value);
        virtual void OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);
        
        virtual void OnMinimize();
        virtual void OnRestore();
        virtual void OnResize(int w,int h);
        virtual void OnExpose();
        virtual void OnExit();
        virtual void OnUser(Uint8 type, int code, void * data1, void * data2);
};

EventHandler::EventHandler() {
}
 
EventHandler::~EventHandler() {
}
 
void EventHandler::OnEvent(SDL_Event* Event) {
    switch(Event->type) {
        case SDL_ACTIVEEVENT:
            switch(Event->active.state) {
                case SDL_APPMOUSEFOCUS:
                    if (Event->active.gain)
                        OnMouseFocus();
                    else
                        OnMouseBlur();
 
                    break;
                    
                case SDL_APPINPUTFOCUS:
                    if ( Event->active.gain )
                        OnInputFocus();
                    else
                        OnInputBlur();
 
                    break;
                    
                case SDL_APPACTIVE:
                    if ( Event->active.gain )
                        OnRestore();
                    else
                        OnMinimize();
 
                    break;
            }
            break;
 
        case SDL_KEYDOWN:
            OnKeyDown(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
            break;
 
        case SDL_KEYUP:
            OnKeyUp(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
            break;
 
        case SDL_MOUSEMOTION:
            OnMouseMove(Event->motion.x,Event->motion.y,Event->motion.xrel,Event->motion.yrel,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
            break;
 
        case SDL_MOUSEBUTTONDOWN:
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT:
                    OnLButtonDown(Event->button.x,Event->button.y);
                    break;
                    
                case SDL_BUTTON_RIGHT:
                    OnRButtonDown(Event->button.x,Event->button.y);
                    break;
                    
                case SDL_BUTTON_MIDDLE:
                    OnMButtonDown(Event->button.x,Event->button.y);
                    break;
            }
            break;
 
        case SDL_MOUSEBUTTONUP:
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT:
                    OnLButtonUp(Event->button.x,Event->button.y);
                    break;
                    
                case SDL_BUTTON_RIGHT:
                    OnRButtonUp(Event->button.x,Event->button.y);
                    break;
                    
                case SDL_BUTTON_MIDDLE:
                    OnMButtonUp(Event->button.x,Event->button.y);
                    break;
            }
            break;
 
        case SDL_JOYAXISMOTION:
            OnJoyAxis(Event->jaxis.which,Event->jaxis.axis,Event->jaxis.value);
            break;
 
        case SDL_JOYBALLMOTION:
            OnJoyBall(Event->jball.which,Event->jball.ball,Event->jball.xrel,Event->jball.yrel);
            break;
 
        case SDL_JOYHATMOTION:
            OnJoyHat(Event->jhat.which,Event->jhat.hat,Event->jhat.value);
            break;
            
        case SDL_JOYBUTTONDOWN:
            OnJoyButtonDown(Event->jbutton.which,Event->jbutton.button);
            break;
 
        case SDL_JOYBUTTONUP:
            OnJoyButtonUp(Event->jbutton.which,Event->jbutton.button);
            break;
 
        case SDL_QUIT:
            OnExit();
            break;
 
        case SDL_SYSWMEVENT:
            //Ignore
            break;
 
        case SDL_VIDEORESIZE:
            OnResize(Event->resize.w,Event->resize.h);
            break;
 
        case SDL_VIDEOEXPOSE:
            OnExpose();
            break;
 
        default:
            OnUser(Event->user.type,Event->user.code,Event->user.data1,Event->user.data2);
            break;
    }
}

void EventHandler::OnInputFocus() {}
void EventHandler::OnInputBlur() {}

void EventHandler::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    IS_KEY_PRESSED[sym] = true;
}

void EventHandler::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    IS_KEY_PRESSED[sym] = false;
}

void EventHandler::OnMouseFocus() {}
void EventHandler::OnMouseBlur() {}
void EventHandler::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) {}
void EventHandler::OnMouseWheel(bool Up, bool Down) {}
void EventHandler::OnLButtonDown(int mX, int mY) {}
void EventHandler::OnLButtonUp(int mX, int mY) {}
void EventHandler::OnRButtonDown(int mX, int mY) {}
void EventHandler::OnRButtonUp(int mX, int mY) {}
void EventHandler::OnMButtonDown(int mX, int mY) {}
void EventHandler::OnMButtonUp(int mX, int mY) {}
void EventHandler::OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value) {}
void EventHandler::OnJoyButtonDown(Uint8 which,Uint8 button) {}
void EventHandler::OnJoyButtonUp(Uint8 which,Uint8 button) {}
void EventHandler::OnJoyHat(Uint8 which,Uint8 hat,Uint8 value) {}
void EventHandler::OnJoyBall(Uint8 which,Uint8 ball,Sint16 xrel,Sint16 yrel) {}
void EventHandler::OnMinimize() {}
void EventHandler::OnRestore() {}
void EventHandler::OnResize(int w,int h) {}
void EventHandler::OnExpose() {}
void EventHandler::OnExit() {}
void EventHandler::OnUser(Uint8 type, int code, void* data1, void* data2) {}

#endif
