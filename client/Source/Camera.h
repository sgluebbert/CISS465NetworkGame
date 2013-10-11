#ifndef CAMERA_H
#define CAMERA_H



#include <deque>
#include <SDL.h>

#include "Bullet.h"
#include "Bullet_List.h"
#include "Ship.h"
#include "Surface_Manager.h"



class Camera {
public:
	static Camera* getInstance();

    void Map_To_Viewport(Ship *);
    void Map_To_World(Ship *);
    void Map_To_Viewport(Bullet *);
    void Map_To_World(Bullet *);
    void Draw_Ships(std::deque<Ship *>);
    void Draw_Bullets(Bullet_List *);
    
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

void Camera::Map_To_Viewport(Ship * ship) {
    if (ship == NULL)
        return;

    ship->x -= viewport.x;
    ship->y -= viewport.y;
}

void Camera::Map_To_World(Ship * ship) {
    if (ship == NULL)
        return;

    ship->x += viewport.x;
    ship->y += viewport.y;
}

void Camera::Map_To_Viewport(Bullet * bullet) {
    if (bullet == NULL)
        return;

    bullet->x -= viewport.x;
    bullet->y -= viewport.y;
}

void Camera::Map_To_World(Bullet * bullet) {
    if (bullet == NULL)
        return;

    bullet->x += viewport.x;
    bullet->y += viewport.y;
}

void Camera::Draw_Ships(std::deque<Ship *>  ships) {
    for (int i = 0; i < ships.size(); i++) {
        if (ships[i] == NULL)
            continue;

        Map_To_Viewport(ships[i]);

        SDL_Rect temp_rect = ships[i]->Get_Bounding_Box();
        // std::cout << temp_rect.x << ' ' << temp_rect.y << '\n';
        if (temp_rect.x >= -temp_rect.w / 2 &&
            temp_rect.x - temp_rect.w / 2 <= WINDOW_BOUNDING_BOX.w &&
            temp_rect.y >= -temp_rect.h / 2 &&
            temp_rect.y - temp_rect.h / 2 <= WINDOW_BOUNDING_BOX.h)
           ships[i]->Draw();

        Map_To_World(ships[i]);
    }
}

void Camera::Draw_Bullets(Bullet_List * temp_list) {
    for (int i = 0; i < temp_list->bullets.size(); i++) {
        if (temp_list->bullets[i] == NULL)
            continue;

        Map_To_Viewport(temp_list->bullets[i]);

        SDL_Rect temp_rect = temp_list->bullets[i]->Get_Bounding_Box();
        if (temp_rect.x >= -temp_rect.w / 2 &&
            temp_rect.x - temp_rect.w / 2 <= WINDOW_BOUNDING_BOX.w &&
            temp_rect.y >= -temp_rect.h / 2 &&
            temp_rect.y - temp_rect.h / 2 <= WINDOW_BOUNDING_BOX.h)
           temp_list->bullets[i]->Draw();

        Map_To_World(temp_list->bullets[i]);
    }
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

SDL_Rect Camera::Get_Viewport() {
	return viewport;
}



#endif
