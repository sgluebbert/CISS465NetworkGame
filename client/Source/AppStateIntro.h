#ifndef APPSTATEINTRO_H
#define APPSTATEINTRO_H

#include "AppStateBase.h"
#include "Font.h"
#include "Sound.h"
#include "System.h"
 
class AppStateIntro : public AppStateBase {
private:
        static AppStateBase * instance;
        
        static const char * MUSIC_FILENAME;
        
        const int number_of_bumpers;
        std::string bumper_titles[3];
        int current_bumper;

        const double bumber_length;
        const double bumper_duration;
        const double transition_point;
        double bumper_timer;

        const double color_change_rate;
        double current_color;
        
        // Font title_font;
        
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

const char * AppStateIntro::MUSIC_FILENAME = "./Sound/Music/Intro.ogg";

AppStateIntro::AppStateIntro()
    :number_of_bumpers(3), bumber_length(32.0),
     bumper_duration(bumber_length / 3.0), transition_point(bumber_length / 6.0),
     color_change_rate(255 / transition_point) {
}

void AppStateIntro::Initialize() {
    bumper_titles[0] = "CISS465 Project";
    bumper_titles[1] = "By Sam Luebbert";
    bumper_titles[2] = "And Trevin Teacutter";
    current_bumper = 0;
    bumper_timer = 0.0;
    current_color = 0.0;
    // title_font = Font("./Font/microsbe.ttf", 50);
    
    MUSIC_STREAM.load(MUSIC_FILENAME);
    MUSIC_STREAM.play();
}

void AppStateIntro::Events(SDL_Event * Event) {
    OnEvent(Event);
}

void AppStateIntro::Update() {
        bumper_timer += GetTimePerFrame();

        if (bumper_timer > bumper_duration) {
            current_bumper += 1;
            bumper_timer = 0.0;
        }

        if (current_bumper >= number_of_bumpers)
            AppStateEvent::New_Event(APPSTATE_MENU);

        if (bumper_timer < transition_point)
            current_color += color_change_rate * GetTimePerFrame();
        else
            current_color -= color_change_rate * GetTimePerFrame();

        if (current_color > 255)
            current_color = 255;
        else if (current_color < 0)
            current_color = 0;
}

void AppStateIntro::Draw() {
    FontManager * fontManger = FontManager::getInstance();

    SDL_Color color = {int(current_color), int(current_color), int(current_color), 0};
    SDL_Surface * temp_surf = fontManger->Render(fontManger->title_font_48, bumper_titles[current_bumper], color);
    SDL_Rect temp_rect = temp_surf->clip_rect;

    Surface::Blit(WINDOW, temp_surf, (WINDOW_BOUNDING_BOX.w - temp_rect.w) / 2.0, (WINDOW_BOUNDING_BOX.h - temp_rect.h) / 2.0);
    SDL_FreeSurface(temp_surf);
}

void AppStateIntro::Cleanup() {
    MUSIC_STREAM.stop();
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
    case SDLK_SPACE:  AppStateEvent::New_Event(APPSTATE_MENU);    break;
    default:    break;
    }
}

void AppStateIntro::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
    default:    break;
    }
}

#endif
