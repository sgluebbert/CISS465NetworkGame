#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H
 
 
 
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



Application_Event * Application_Event::event = NULL;



Application_Event::Application_Event() {
}
    
Application_Event::Application_Event(Application_Flag new_code) {
    event_code = new_code;
}
    
Application_Flag Application_Event::Get_Event_Code() {
    return event_code;
}
    
void Application_Event::New_Event(Application_Flag new_code) {
    event = new Application_Event(new_code);
}
    
Application_Event * Application_Event::Poll_Event() {
    Application_Event * temp;
        
    if (event == NULL)
        return NULL;
            
    temp = event;
    event = NULL;
    return temp;
}



#endif
