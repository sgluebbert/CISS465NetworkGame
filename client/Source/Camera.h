#ifndef CAMERA_H
#define CAMERA_H



#include <list>
#include <SDL.h>

#include "Entity.h"
#include "Surface.h"



class Camera {
public:
	static Camera* getInstance();

    void Map_To_Viewport(Entity *);
    void Map_To_World(Entity *);
    
    void Set_Viewport(const SDL_Rect &);
    void Set_Viewport(double, double, double, double);
    SDL_Rect Get_Viewport();

private:
    Camera();
    SDL_Rect viewport;

	static bool instanceFlag;
	static Camera *single;
};



bool Camera::instanceFlag = false;
Camera * Camera::single = NULL;



Camera * Camera::getInstance() {
	if (!instanceFlag) {
		single = new Camera();
		instanceFlag = true;
		return single;
	}
	else
		return single;
}

Camera::Camera() {
    viewport.x = 0;
    viewport.y = 0;
	viewport.w = WINDOW_BOUNDING_BOX.w;
	viewport.h = WINDOW_BOUNDING_BOX.h;
}

void Camera::Map_To_Viewport(/*Camera * camera, */Entity * entity) {
    // if (camera == NULL)
    //     return;
    if (entity == NULL)
        return;

    entity->x -= viewport.x;
    entity->y -= viewport.y;
}

void Camera::Map_To_World(/*Camera * camera, */Entity * entity) {
    // if (camera == NULL)
    //     return;
    if (entity == NULL)
        return;

    // entity->x += camera->Get_Viewport().x;
    // entity->y += camera->Get_Viewport().y;
    entity->x += viewport.x;
    entity->x += viewport.y;
}
    
void Camera::Set_Viewport(const SDL_Rect & nviewport) {
    viewport = nviewport;
    // if (viewport.x < 0)
    // 	viewport.x = 0;
    // if (viewport.y < 0)
    // 	viewport.y = 0;
    // if (viewport.x + viewport.w > ROOM_WIDTH)
    // 	viewport.x = ROOM_WIDTH - viewport.w;
    // if (viewport.y + viewport.h > ROOM_HEIGHT)
    // 	viewport.y = ROOM_HEIGHT - viewport.h;
}

void Camera::Set_Viewport(double x, double y, double w, double h) {
    viewport.x = x;
    viewport.y = y;
    viewport.w = w;
    viewport.h = h;
    // if (viewport.x < 0)
    // 	viewport.x = 0;
    // if (viewport.y < 0)
    // 	viewport.y = 0;
    // if (viewport.x + viewport.w > ROOM_WIDTH)
    // 	viewport.x = ROOM_WIDTH - viewport.w;
    // if (viewport.y + viewport.h > ROOM_HEIGHT)
    // 	viewport.y = ROOM_HEIGHT - viewport.h;
}

SDL_Rect Camera::Get_Viewport() {
	return viewport;
}



#endif
