#ifndef CAMERA_H
#define CAMERA_H



#include "Circle.h"
#include "Drawing/Drawable.h"
#include "Rect.h"



class Camera {
public:
    static Camera * getInstance();

    void Map_To_Viewport(Drawable *);
    void Map_To_World(Drawable *);

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
