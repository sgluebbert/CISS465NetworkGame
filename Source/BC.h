#ifndef BC_H
#define BC_H



#include "BV.h"



class BC : public BV {
public:
	double x, y, r;



	BC()
		: x(0), y(0), r(0) {}

	BC(double _x, double _y, double _r)
		: x(_x), y(_y), r(_r) {}



	void Update(double dt, double _x, double _y) {
		x = _x;
		y = _y;
	}

	void Draw() {
	}

private:	
};



std::ostream & operator<<(std::ostream & cout, const BC & rhs) {
    std::cout << "Position: [" << rhs.x << ", " << rhs.y << "]\t";
    std::cout << "Radius: " << rhs.r << std::endl;

    return cout;
}



#endif