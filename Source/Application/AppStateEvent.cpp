#include "AppStateEvent.h"



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