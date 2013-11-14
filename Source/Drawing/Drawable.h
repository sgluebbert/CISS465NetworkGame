#ifndef DRAWABLE_H
#define DRAWABLE_H



#include <deque>

#include "../Rect.h"
#include "Texture.h"



class Drawable {
public:
	virtual void Draw() = 0;

    Rect<float> drawing_box;
    Texture * texture;

    float draw_scale, draw_angle;

    static std::deque<Drawable *> objects;
};



#endif