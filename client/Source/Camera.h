#ifndef CAMERA_H
#define CAMERA_H

#include <list>
#include <SDL.h>

#include "Surface.h"

class Camera {
public:
	static Camera* getInstance();
    
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
Camera* Camera::single = NULL;

Camera* Camera::getInstance() {
	if (!instanceFlag) {
		single = new Camera();
		instanceFlag = true;
		return single;
	}
	else
		return single;
}

Camera::Camera() {
	viewport.w = WINDOW->w;
	viewport.h = WINDOW->h;
}
    
void Camera::Set_Viewport(const SDL_Rect & nviewport) {
    viewport = nviewport;
    if (viewport.x < 0)
    	viewport.x = 0;
    if (viewport.y < 0)
    	viewport.y = 0;
    if (viewport.x + viewport.w > ROOM_WIDTH)
    	viewport.x = ROOM_WIDTH - viewport.w;
    if (viewport.y + viewport.h > ROOM_HEIGHT)
    	viewport.y = ROOM_HEIGHT - viewport.h;
}

void Camera::Set_Viewport(double x, double y, double w, double h) {
    viewport.x = x;
    viewport.y = y;
    viewport.w = w;
    viewport.h = h;
    if (viewport.x < 0)
    	viewport.x = 0;
    if (viewport.y < 0)
    	viewport.y = 0;
    if (viewport.x + viewport.w > ROOM_WIDTH)
    	viewport.x = ROOM_WIDTH - viewport.w;
    if (viewport.y + viewport.h > ROOM_HEIGHT)
    	viewport.y = ROOM_HEIGHT - viewport.h;
}

SDL_Rect Camera::Get_Viewport()
{
	return viewport;
}

#endif
