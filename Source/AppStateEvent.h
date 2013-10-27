#ifndef APPSTATEEVENT_H
#define APPSTATEEVENT_H
 
enum AppState {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_MENU,
    APPSTATE_GAME
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

AppStateEvent * AppStateEvent::event = NULL;

AppStateEvent::AppStateEvent() {
}
    
AppStateEvent::AppStateEvent(AppState new_code) {
    event_code = new_code;
}
    
AppState AppStateEvent::Get_Event_Code() {
    return event_code;
}
    
void AppStateEvent::New_Event(AppState new_code) {
    event = new AppStateEvent(new_code);
}
    
AppStateEvent * AppStateEvent::Poll_Event() {
    AppStateEvent * temp;
        
    if (event == NULL)
        return NULL;
            
    temp = event;
    event = NULL;
    return temp;
}

#endif
