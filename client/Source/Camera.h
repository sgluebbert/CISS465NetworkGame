#ifndef CAMERA_H
#define CAMERA_H

#include <list>
#include <SDL.h>

#include "Entity.h"

class Camera {
public:
    Camera();
    Camera(const SDL_Rect &);
    ~Camera();
    
    void Set_Viewport(const SDL_Rect &);
    void Set_Viewport(double, double, double, double);
    
    void Map_To_Window(double &, double &);
    void Map_To_World(double &, double &);
    
    //void Events(SDL * Event);
    void Draw();
    void Update();
    
private:
    SDL_Rect viewport;
    std::list<Entity *> drawable;
    
};



Camera::Camera() {
}

Camera::~Camera() {
    //Probably not needed, but just to be sure
    drawable.clear();
}
    
void Camera::Set_Viewport(const SDL_Rect & nviewport) {
    viewport = nviewport;
}

void Camera::Set_Viewport(double x, double y, double w, double h) {
    viewport.x = x;
    viewport.y = y;
    viewport.w = w;
    viewport.h = h;
}

void Camera::Map_To_Window(double & world_x, double & world_y) {
    world_x = viewport.x - world_x;
    world_y = viewport.y - world_y;
}

void Camera::Map_To_World(double & screen_x, double & screen_y) {
    screen_x = viewport.x + screen_x;
    screen_y = viewport.y + screen_y;
}

void Camera::Draw() {
    Entity * temp;
    
    while (!drawable.empty()) {
        temp = drawable.front();
        drawable.pop_front();
        temp->Draw();
    }
}

void Camera::Update() {
    //This is where you find all objects that collide with the viewport
    //and add them to the drawable list to be drawn.
}

#endif
