#ifndef APPSTATEINTRO_H
#define APPSTATEINTRO_H



#include "AppStateBase.h"
#include "Clock.h"
#include "System.h"
 
 
 
class AppStateIntro : public AppStateBase {
private:
        static AppStateBase * instance;
        
        const int number_of_bumpers;
        std::string bumper_titles[3];
        int current_bumper;

        const double bumber_length;
        const double bumper_duration;
        const double transition_point;
        double bumper_timer;

        const double color_change_rate;
        double current_color;
        
        AppStateIntro();
public:
 
        void Initialize();
        void Events(SDL_Event *);
        void Update();
        void Draw();
        void Cleanup();
        
        static AppStateBase * GetInstance();
 
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
};



AppStateBase * AppStateIntro::instance = NULL;



AppStateIntro::AppStateIntro()
    :number_of_bumpers(3),
     bumber_length(32.0),
     bumper_duration(bumber_length / 3.0),
     transition_point(bumber_length / 6.0),
     color_change_rate(255 / transition_point) {
}

void AppStateIntro::Initialize() {
    bumper_titles[0] = "CISS465 Project";
    bumper_titles[1] = "By Sam Luebbert";
    bumper_titles[2] = "And Trevin Teacutter";
    current_bumper = 0;
    bumper_timer = 0.0;
    current_color = 0.0;
    
    sound_manager->Load_Music("./Sound/Music/Intro.ogg");
    sound_manager->Play_Music();
}

void AppStateIntro::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateIntro::Update() {
        double temp_delta = Clock::Frame_Control.Get_Time_Per_Frame();
        bumper_timer += temp_delta;

        if (bumper_timer > bumper_duration) {
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

void AppStateIntro::Draw() {
    Color color(current_color / 255, current_color / 255, current_color / 255, 1);

    Text temp(bumper_titles[current_bumper].c_str(), TextureManager::GetInstance()->fonts.font_Impact_20, color);
    SDL_Rect temp_rect;
    temp_rect.w = temp.width;
    temp_rect.h = temp.height;
    temp_rect.x = (WINDOW_BOUNDING_BOX.w - temp_rect.w) / 2.0;
    temp_rect.y = (WINDOW_BOUNDING_BOX.h - temp_rect.h) / 2.0;

    temp.Draw(temp_rect.x, temp_rect.y);
    glColor4f(1, 1, 1, 1);
}

void AppStateIntro::Cleanup() {
    sound_manager->Stop_Music();
}

AppStateBase * AppStateIntro::GetInstance() {
    if (instance != NULL)
        delete instance;
        
    instance = new AppStateIntro();
    return instance;
}

void AppStateIntro::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    case SDLK_TAB:
    case SDLK_SPACE:
        if (current_bumper < number_of_bumpers - 1) {
            current_bumper += 1;
            current_color = 0;
            bumper_timer = 0.0;
        }
        else
            AppStateEvent::New_Event(APPSTATE_MENU);
        break;
    default:    break;
    }
}

void AppStateIntro::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:    break;
    }
}



#endif
