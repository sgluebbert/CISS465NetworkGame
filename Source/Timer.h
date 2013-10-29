#ifndef TIMER_H
#define TIMER_H



#include <SDL.h>

#include "System.h"



class Timer {
    public:
        static Timer Frame_Control;
        
        Timer();
        void Update();

        int Get_FPS();
        double Get_Time_Per_Frame();

    private:
        int old_time;
        int last_time;

        double speed_factor;
        double delta;

        int num_of_frames;
        int frames;
};



Timer Timer::Frame_Control;



Timer::Timer() {
    old_time = 0;
    last_time = 0;

    speed_factor = 0.0;

    frames = 0;
    num_of_frames = 0;
}

void Timer::Update() {
    if(old_time + 1000 < SDL_GetTicks()) {
        old_time = SDL_GetTicks();
        
        num_of_frames = frames;
        frames = 0;
    }

    delta = 1.0 / FPS_LIMIT;
    speed_factor = ((SDL_GetTicks() - last_time) / 1000.0);
    last_time = SDL_GetTicks();
    frames++;
    
    int delay = (delta - speed_factor) * 1000;
    if (delay > 0)
        SDL_Delay(delay);
        
}

int Timer::Get_FPS() {
    return num_of_frames;
}

double Timer::Get_Time_Per_Frame() {
    if (speed_factor > delta)
        return speed_factor;
    else
        return delta;
}



#endif
