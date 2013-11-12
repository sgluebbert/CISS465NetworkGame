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
	double Get_Interval();
	double Get_Progress();

private:
	bool active;
	
	double timer;
	double interval;
};



#endif