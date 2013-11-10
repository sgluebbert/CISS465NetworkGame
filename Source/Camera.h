#ifndef CAMERA_H
#define CAMERA_H



#include <deque>
#include <SDL.h>

#include "Circle.h"
#include "Entity.h"
#include "Rect.h"
#include "TextureManager.h"



class Camera {
public:
	static Camera * getInstance();

    void Map_To_Viewport(Entity *);
    void Map_To_World(Entity *);

    void Map_To_Viewport(Circle<double> *);
    void Map_To_World(Circle<double> *);

    void Map_To_Viewport(Rect<double> *);
    void Map_To_World(Rect<double> *);
    
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
