#include "Timer.h"



Timer::Timer()
	: timer(0.0), interval(0.0), active(false) {}

Timer::Timer(double _timer)
	: timer(_timer), interval(_timer), active(false) {}

void Timer::Start() {
	active = true;
	timer = 0.0;
}

bool Timer::Ended() {
	return !active;
}

void Timer::Update(double dt) {
	if (active)
		timer += dt;
	if (timer >= interval)
		active = false;
}

void Timer::Set_Interval(double _interval) {
	interval = _interval;
}

double Timer::Get_Interval() {
	return interval;
}

double Timer::Get_Progress() {
	return (timer / interval);
}