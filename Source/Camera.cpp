#include "Camera.h"



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


    
void Camera::Set_Viewport(const Rect<double> & nviewport) {
    viewport = nviewport;
    /*if (viewport.x < 0)
    	viewport.x = 0;
    if (viewport.y < 0)
    	viewport.y = 0;
    if (viewport.x + viewport.w > ROOM_WIDTH)
    	viewport.x = ROOM_WIDTH - viewport.w;
    if (viewport.y + viewport.h > ROOM_HEIGHT)
    	viewport.y = ROOM_HEIGHT - viewport.h;*/
}

void Camera::Set_Viewport(double x, double y, double w, double h) {
    viewport.x = x;
    viewport.y = y;
    viewport.w = w;
    viewport.h = h;
    /*if (viewport.x < 0)
    	viewport.x = 0;
    if (viewport.y < 0)
    	viewport.y = 0;
    if (viewport.x + viewport.w > ROOM_WIDTH)
    	viewport.x = ROOM_WIDTH - viewport.w;
    if (viewport.y + viewport.h > ROOM_HEIGHT)
    	viewport.y = ROOM_HEIGHT - viewport.h;*/
}

Rect<double> Camera::Get_Viewport() {
	return viewport;
}