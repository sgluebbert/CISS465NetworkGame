#ifndef ANIMATION_H
#define ANIMATION_H
 
#include <SDL.h>
 
class Animation {
    protected:
        int CurrentFrame;
        int FrameInc;
		
        int FrameRate; //Milliseconds
        long OldTime;
 
	public:
        int MaxFrames;
        bool Oscillate;
        
        Animation();
        void OnAnimate();
		
        void SetFrameRate(int Rate);
        void SetCurrentFrame(int Frame);
        int GetCurrentFrame();
};

Animation::Animation() {
    CurrentFrame = 0;
    MaxFrames = 0;
    FrameInc = 1;
 
    FrameRate = 100; //Milliseconds
    OldTime = 0;
 
    Oscillate = false;
}
 
void Animation::OnAnimate() {
    if(OldTime + FrameRate > SDL_GetTicks())
        return;
 
    OldTime = SDL_GetTicks();
    CurrentFrame += FrameInc;
 
    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= MaxFrames)
                FrameInc = -FrameInc;
        }
		else {
            if(CurrentFrame <= 0)
                FrameInc = -FrameInc;
        }
    }
	else {
        if(CurrentFrame >= MaxFrames)
            CurrentFrame = 0;
    }
}
 
void Animation::SetFrameRate(int Rate) {
    FrameRate = Rate;
}
 
void Animation::SetCurrentFrame(int Frame) {
    if(Frame < 0 || Frame >= MaxFrames) return;
    CurrentFrame = Frame;
}
 
int Animation::GetCurrentFrame() {
    return CurrentFrame;
}

#endif
