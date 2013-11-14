#ifndef CIRCLE_H
#define CIRCLE_H



template <class T>
class Circle {
public:
	T x, y, r;

	Circle()
		: x(0), y(0), r(0) {}

	Circle(T _x, T _y, T _r)
		: x(_x), y(_y), r(_r) {}



	void Update(T x_offset, T y_offset, T r_offset = 0) {
		x += x_offset;
		y += y_offset;
		r += r_offset;
	}

	void Draw() {
	}
};



#endif