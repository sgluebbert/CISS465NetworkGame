#include "Clock.h"



Clock Clock::Frame_Control;



Clock::Clock() {
    old_time = 0;
    last_time = 0;

    speed_factor = 0.0;

    frames = 0;
    num_of_frames = 0;
}

void Clock::Update() {
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

int Clock::Get_FPS() {
    return num_of_frames;
}

double Clock::Get_Time_Per_Frame() {
    if (speed_factor > delta)
        return speed_factor;
    else
        return delta;
}