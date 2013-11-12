#ifndef DRAWABLE_H
#define DRAWABLE_H



#include "Rect.h"
#include "Texture.h"



class Drawable {
public:
	virtual void Draw() = 0;

    Rect<float> drawing_box;
    Texture * texture;

    float draw_scale, draw_angle;
};



#endif