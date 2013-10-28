#ifndef TIMER_H
#define TIMER_H



class Timer {
public:
	Timer();
	Timer(double);

	void Start();
	bool Ended();

	void Update(double);

	void Set_Interval(double);
	double Get_Progress();

private:
	double timer;
	double interval;
};



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



#endif