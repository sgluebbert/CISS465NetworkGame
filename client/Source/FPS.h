#ifndef FPS_H
#define FPS_H

#include <SDL.h>

#include "System.h"

class FPS {
    private:
        int OldTime;
        int LastTime;

        double SpeedFactor;

        int NumFrames;
        int Frames;

    public:
        static FPS FPSControl;
        
        FPS();
        void Update();

        int GetFPS();
        double GetSpeedFactor();
};

FPS FPS::FPSControl;

FPS::FPS() {
    OldTime = 0;
    LastTime = 0;

    SpeedFactor = 0.0;

    Frames = 0;
    NumFrames = 0;
}

void FPS::Update() {
    if(OldTime + 1000 < SDL_GetTicks()) {
        OldTime = SDL_GetTicks();
        
        NumFrames = Frames;
        Frames = 0;
    }

    SpeedFactor = ((SDL_GetTicks() - LastTime) / 1000.0f);
    LastTime = SDL_GetTicks();
    Frames++;
    
    int delay = (GetTimePerFrame() - SpeedFactor) * 1000;
    if (delay > 0)
        SDL_Delay(delay);
        
}

int FPS::GetFPS() {
    return NumFrames;
}

double FPS::GetSpeedFactor() {
    return SpeedFactor;
}

#endif
