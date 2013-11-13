#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H



#include <cstdlib>
 
 
 
enum Application_Flag {
    EXIT_FLAG,
    ERROR_FLAG
};



class Application_Event {
private:
    static Application_Event * event;
    Application_Flag event_code;
    
public:
    Application_Event();
    Application_Event(Application_Flag);
    
    Application_Flag Get_Event_Code();
    
    static void New_Event(Application_Flag);
    static Application_Event * Poll_Event();
};



#endif
