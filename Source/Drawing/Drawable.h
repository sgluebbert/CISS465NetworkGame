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
    Texture * texture_c; // used for color outlines

    float draw_scale, draw_angle;
    int draw_priority;//0-9: 0 being lowest priority and 9 being the highest priority | think of this as drawing in layers

    //Need to turn this into a priority queue
    static std::deque<Drawable *> objects;
};



#endif