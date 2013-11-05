#include "AppStateMenu.h"



AppStateBase * AppStateMenu::instance = NULL;



AppStateMenu::AppStateMenu() {
}

void AppStateMenu::Initialize() {
    background_surf = surface_manager->background_main_menu;
    
    // sound_manager->Load_Music("./Sound/Music/Main_Menu.ogg");
    // sound_manager->Play_Music();
    
    main_menu.Add_Option("Play");
    main_menu.Add_Option("Test");
    main_menu.Add_Option("Exit");
    
    main_menu.Center_To_Window();
}

void AppStateMenu::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateMenu::Update() {
}

void AppStateMenu::Draw() {
    background_surf->DrawAtRect(0, 0, WINDOW_BOUNDING_BOX.w + 400, WINDOW_BOUNDING_BOX.h + 400);
    main_menu.Draw();
}

void AppStateMenu::Cleanup() {
    sound_manager->Stop_Music();
}

AppStateBase * AppStateMenu::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateMenu();
    return instance;
}

void AppStateMenu::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_UP:       main_menu.Move_Previous();                  break;
    case SDLK_DOWN:     main_menu.Move_Next();                      break;
    case SDLK_RETURN:
        switch(main_menu.Select()) {
        case 0:     AppStateEvent::New_Event(APPSTATE_GAME);            break;
        case 1:     AppStateEvent::New_Event(APPSTATE_TEST);            break;
        default:    AppStateEvent::New_Event(APPSTATE_NONE);            break;
        }
        break;
    default:
        break;
    }
}

void AppStateMenu::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:
        break;
    }
}