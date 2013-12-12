#include "Application.h"



Application::Application() {
    WINDOW = NULL;
    WINDOW_BOUNDING_BOX = DEFAULT_WINDOW_BOUNDING_BOX;
    is_running = true;
}



bool Application::Execute(bool gui) {
    if (!Initialize(gui))
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



bool Application::Initialize(bool _gui) {
    gui = _gui;
    if (gui && !Initialize_System())
        return false;

    Initialize_Trig_Table();

    AppStateManager::Initialize(gui);\
    
    return true;
}

void Application::Events(SDL_Event * Event) {
    //Sends Events to appropriate functions
    EventHandler::OnEvent(Event);
    AppStateManager::Events(Event);
    HUDManager::handleEvent(Event);
}

void Application::Draw() {
    if (!gui)
        return;
    
    glClear( GL_COLOR_BUFFER_BIT );
    glLoadIdentity();

    AppStateManager::Draw();
    HUDManager::draw();

    SDL_GL_SwapBuffers();
}

void Application::Update() {
    Application_Event * appFlag = Application_Event::Poll_Event();
    
    if (appFlag != NULL)
        is_running = false;

    AppStateManager::Update();
    HUDManager::update();

    Clock::Frame_Control.Update();

    char Buffer[255];
    sprintf(Buffer, "FPS: %d", Clock::Frame_Control.Get_FPS());
    SDL_WM_SetCaption(WINDOW_TITLE, Buffer);
}

void Application::Cleanup() {
    AppStateManager::Cleanup();
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
    surface_manager->Resize(w, h);
}

void Application::OnExpose() {
}

void Application::OnExit() {
    is_running = false;
}

void Application::OnUser(Uint8 type, int code, void* data1, void* data2) {
}
