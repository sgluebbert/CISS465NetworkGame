#ifndef CIRCLE_H
#define CIRCLE_H



#include <iostream>



template <class T>
class Circle {
public:
	T x, y, r;

	Circle()
		: x(0), y(0), r(0) {}

	Circle(T _x, T _y, T _r)
		: x(_x), y(_y), r(_r) {}



	void Update(double dt, T _x, T _y) {
		x = _x;
		y = _y;
	}

	void Draw() {
	}
};



template <class T>
std::ostream & operator<<(std::ostream & cout, const Circle<T> & rhs) {
    std::cout << "Position: [" << rhs.x << ", " << rhs.y << "]\t";
    std::cout << "Radius: " << rhs.r << std::endl;

    return cout;
}



#endif