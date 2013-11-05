#ifndef CAMERA_H
#define CAMERA_H



#include <deque>
#include <SDL.h>

#include "Rect.h"
#include "TextureManager.h"



class Camera {
public:
	static Camera * getInstance();
    
    void Set_Viewport(const Rect<double> &);
    void Set_Viewport(double, double, double, double);
    Rect<double> Get_Viewport();

private:
    Camera();
    Rect<double> viewport;

	static bool instanceFlag;
	static Camera *single;
};



#endif
