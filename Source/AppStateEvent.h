#ifndef APPSTATEEVENT_H
#define APPSTATEEVENT_H



#include <cstdlib>



enum AppState {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_MENU,
    APPSTATE_GAME,
    APPSTATE_GAME_SERVER,
    APPSTATE_TEST
};



class AppStateEvent {
private:
    static AppStateEvent * event;
    AppState event_code;
    
public:
    AppStateEvent();
    AppStateEvent(AppState);
    
    AppState Get_Event_Code();
    
    static void New_Event(AppState);
    static AppStateEvent * Poll_Event();
};



#endif
