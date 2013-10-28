#ifndef AABB_H
#define AABB_H



#include "BV.h"



class AABB : public BV {
public:
	double x, y, w, h;



	AABB()
		: x(0), y(0), w(0), h(0) {}

	AABB(double _x, double _y, double _w, double _h, bool centered = false) {
		if (centered) {
			w = _w;
			h = _h;
			x = _x - (w / 2.0);
			y = _y - (h / 2.0);
		}
		else {
			x = _x;
			y = _y;
			w = _w;
			h = _h;
		}
	}



	void Update(double dt, double _x, double _y) {
		x = _x;
		y = _y;
	}

	void Draw() {
	}

private:
};



std::ostream & operator<<(std::ostream & cout, const AABB & rhs) {
    std::cout << "Position: [" << rhs.x << ", " << rhs.y << "]\t";
    std::cout << "Bounds: [" << rhs.w << ", " << rhs.h << ']' << std::endl;

    return cout;
}



#endif