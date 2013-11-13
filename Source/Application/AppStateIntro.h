#ifndef APPSTATEINTRO_H
#define APPSTATEINTRO_H



#include "AppStateBase.h"
#include "../Clock.h"
#include "../System.h"
 
 
 
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



#endif
