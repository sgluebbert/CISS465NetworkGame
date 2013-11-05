#include "Timer.h"



Timer::Timer()
	: timer(0.0), interval(0.0) {}

Timer::Timer(double _timer)
	: timer(_timer), interval(_timer) {}

void Timer::Start() {
	timer = 0.0;
}

bool Timer::Ended() {
	return timer >= interval;
}

void Timer::Update(double dt) {
	if (timer < interval)
		timer += dt;
}

void Timer::Set_Interval(double _interval) {
	interval = _interval;
}

double Timer::Get_Progress() {
	return (timer / interval);
}