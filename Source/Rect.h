#ifndef RECT_H
#define RECT_H



#include <SDL.h>
#include <iostream>



template <class T>
struct Rect {
	T x, y, w, h;

	Rect()
		: x(0), y(0), w(0), h(0) {}

	Rect(T _x, T _y, T _w, T _h, bool centered = false) {
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

	Rect & operator=(SDL_Rect & other) {
		x = other.x;
		y = other.y;
		w = other.w;
		h = other.h;
	}



	bool Intersects(const Rect<T> & rhs) const {
		return Intersects_Horizontally(rhs) && Intersects_Vertically(rhs);
	}

	bool Intersects(double _x, double _y) const {
		return (_x >= x && _x <= x + w &&
				_y >= y && _y <= y + h);
	}

	bool Within(const Rect<T> & rhs) const {
	    return (rhs.x >= x &&
	            rhs.x + rhs.w <= x + w &&
	            rhs.y >= y &&
	            rhs.y + rhs.h <= y + h);
	}



	void Update(T x_offset, T y_offset, T w_offset = 0, T h_offset = 0) {
		x += x_offset;
		y += y_offset;
		w += w_offset;
		h += h_offset;
	}

	void Draw() {
	}

private:
	bool Intersects_Horizontally(const Rect<T> & rhs) const {
	    if (x >= rhs->x &&
	        x <= rhs->x + rhs->w)
	        return true;

	    if (x + w >= rhs->x &&
	        x + w <= rhs->x + rhs->w)
	        return true;

	    return false;
	}

	bool Intersects_Vertically(const Rect<T> & rhs) const {
	    if (y >= rhs->y &&
	        y <= rhs->y + rhs->h)
	        return true;

	    if (y + h >= rhs->y &&
	        y + h <= rhs->y + rhs->h)
	        return true;

	    return false;
	}

};



template <class T>
std::ostream & operator<<(std::ostream & cout, const Rect<T> & rhs) {
    std::cout << "Position: [" << rhs.x << ", " << rhs.y << "]\t";
    std::cout << "Bounds: [" << rhs.w << ", " << rhs.h << ']' << std::endl;

    return cout;
}



#endif