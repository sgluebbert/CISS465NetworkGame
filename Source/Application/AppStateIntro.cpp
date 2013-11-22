#include "AppStateIntro.h"


AppStateBase * AppStateIntro::instance = NULL;


AppStateIntro::AppStateIntro()
    : number_of_bumpers(2),
      bumber_length(32.0),
      bumper_duration(bumber_length / 3.0),
      transition_point(bumber_length / 6.0),
      color_change_rate(255 / transition_point)
{
}

void AppStateIntro::Initialize()
{
    bumper_titles[0] = "STAR CLASH";
    bumper_titles[1] = "By CISS465 Fall 2013";
    current_bumper = 0;
    bumper_timer = 0.0;
    current_color = 0.0;
}

void AppStateIntro::Events(SDL_Event * Event)
{
    OnEvent(Event);
}

void AppStateIntro::Update()
{
    double temp_delta = Clock::Frame_Control.Get_Time_Per_Frame();
    bumper_timer += temp_delta;

    if (bumper_timer > bumper_duration)
    {
        current_bumper += 1;
        bumper_timer = 0.0;
    }
    
    if (current_bumper >= number_of_bumpers)
        AppStateEvent::New_Event(APPSTATE_MENU);
    
    if (bumper_timer < transition_point)
        current_color += color_change_rate * temp_delta;
    else
        current_color -= color_change_rate * temp_delta;
    
    if (current_color > 255)
        current_color = 255;
    else if (current_color < 0)
        current_color = 0;
}

void AppStateIntro::Draw()
{
    Color color(current_color / 255, current_color / 255, current_color / 255, 1);
    
    Text temp(bumper_titles[current_bumper].c_str(),
              TextureManager::GetInstance()->fonts.font_Impact_80, color);

    SDL_Rect temp_rect;
    temp_rect.w = temp.width;
    temp_rect.h = temp.height;
    temp_rect.x = (WINDOW_BOUNDING_BOX.w - temp_rect.w) / 2.0;
    temp_rect.y = (WINDOW_BOUNDING_BOX.h - temp_rect.h) / 2.0;

    temp.Draw(temp_rect.x, temp_rect.y);

    glColor4f(1, 1, 1, 1);
}

void AppStateIntro::Cleanup()
{
}

AppStateBase * AppStateIntro::GetInstance()
{
    if (instance != NULL)
        delete instance;
    
    instance = new AppStateIntro();
    return instance;
}

void AppStateIntro::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
    switch(sym) {
    case SDLK_o:
        std::cout << "Memory Usage: " << Get_Memory_Usage() << "kb" << std::endl;
        break;
    case SDLK_TAB:
    case SDLK_RETURN:
    case SDLK_SPACE:
        if (current_bumper < number_of_bumpers - 1)
        {
            current_bumper += 1;
            current_color = 0;
            bumper_timer = 0.0;
        }
        else
            AppStateEvent::New_Event(APPSTATE_MENU);
        break;
    default:
        break;
    }
}

void AppStateIntro::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
    switch(sym) {
    default:
        break;
    }
}
