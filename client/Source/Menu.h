#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <vector>

#include "Font.h"
#include "Surface.h"
#include "System.h"

class Menu {
public:
    Menu();
    Menu(std::string);
    Menu(std::string[], int);
    
    void Set_Defaults();
    void Add_Option(std::string);
    void Center_To_Window();
    
    void Set_Position(int, int);
    void Set_Size(int, int);
    void Set_Offset(int, int);
    void Set_Orientation(bool);
    void Set_Background(SDL_Surface *);
    
    void Draw();
        
    void Move_Next();
    void Move_Previous();
    int Select();
    
protected:
    std::vector<std::string> menu_options;
    int selected_option;
    
    int menu_x, menu_y;
    int margin_x, margin_y;
    int offset_x, offset_y;
    
    bool vertically_oriented;
    
    SDL_Surface * option_background;
    SDL_Rect option_rect;
    // Font menu_font;
};

Menu::Menu() {
    Set_Defaults();
}

Menu::Menu(std::string newOption) {
    Set_Defaults();
    Add_Option(newOption);
}

Menu::Menu(std::string * newOptions, int size) {
    Set_Defaults();
    for (int i = 0; i < size; i++)
        Add_Option(newOptions[i]);
}

void Menu::Set_Defaults() {
    selected_option = -1;
    menu_x = 0;
    menu_y = 0;
    offset_x = 0;
    offset_y = 0;
    margin_x = 0;
    margin_y = 0;
    option_rect.w = 100;
    option_rect.h = 50;
    vertically_oriented = true;
    option_background = NULL;
    // menu_font = Font("./Font/microsbe.ttf", 24);
}

void Menu::Add_Option(std::string newOption) {
    menu_options.push_back(newOption);
    selected_option = 0;
}

void Menu::Center_To_Window() {
    int num = menu_options.size();
    
    if (num == 0)
        return;
        
    if (num == 1) {
        menu_x = (WINDOW_BOUNDING_BOX.w - option_rect.w) / 2.0;
        menu_y = (WINDOW_BOUNDING_BOX.w - option_rect.h) / 2.0;
        return;
    }
    
    double tempW = 0.0;
    double tempH = 0.0;
    
    if (vertically_oriented) {
        tempW += option_rect.w;
        tempW += offset_x * (num - 1);
        tempH += option_rect.h * num;
        tempH += margin_y * (num - 1);
        }
    else {
        tempW += option_rect.w * num;
        tempW += margin_x * (num - 1);
        tempW += option_rect.h;
        tempH += offset_y * (num - 1);
    }
    
    menu_x = (WINDOW_BOUNDING_BOX.w - tempW) / 2.0;
    menu_y = (WINDOW_BOUNDING_BOX.h - tempH) / 2.0;
    return;
}

void Menu::Set_Position(int newX, int newY) {
    menu_x = newX;
    menu_y = newY;
}

void Menu::Set_Size(int newW, int newH) {
    option_rect.w = newW;
    option_rect.h = newH;
}

void Menu::Set_Offset(int newoffset_x, int newoffset_y) {
    offset_x = newoffset_x;
    offset_y = newoffset_y;
}

void Menu::Set_Background(SDL_Surface * SurfSrc) {
    if (option_background != NULL)
        delete option_background;
        
    option_background = SurfSrc;
}

void Menu::Set_Orientation(bool newFlag) {
    vertically_oriented = newFlag;
}

void Menu::Draw() {
    int tempX = menu_x;
    int tempY = menu_y;
    
    for (int i = 0; i < menu_options.size(); i++) {
        option_rect.x = tempX;
        option_rect.y = tempY;
        
        option_rect.x += 5;
        option_rect.y += 5;
        option_rect.w -= 5;
        option_rect.h -= 5;
        if (selected_option == i)
            Surface::DrawRect(WINDOW, option_rect, CYAN);
        else
            Surface::DrawRect(WINDOW, option_rect, GRAY);
        option_rect.x -= 5;
        option_rect.y -= 5;
        option_rect.w += 5;
        option_rect.h += 5;
        
        //if (option_background != NULL)
            //SDL_BlitSurface(option_background, &option_rect, WINDOW, NULL);
        FontManager *fontManager = FontManager::getInstance();
        SDL_Surface *temp_surf = fontManager->Render(fontManager->menu_font_24, menu_options[i], BLACK);
        // SDL_Rect temp_rect = temp_surf->clip_rect;
        
        // temp_rect.x = (WINDOW_BOUNDING_BOX.w - temp_rect.w) / 2.0;
        // temp_rect.y = (WINDOW_BOUNDING_BOX.h - temp_rect.h) / 2.0;
        
        // SDL_BlitSurface(WINDOW, &WINDOW_BOUNDING_BOX, temp_surf, &temp_rect);
        Surface::Blit(WINDOW, temp_surf, (WINDOW_BOUNDING_BOX.w - temp_surf->w) / 2.0, option_rect.y + temp_surf->h / 2.0);
        SDL_FreeSurface(temp_surf);
        
        if (vertically_oriented) {
            tempX += offset_x;
            tempY += (option_rect.h + margin_y);
        }
        else {
            tempX += (option_rect.w + margin_x);
            tempY += offset_y;
        }
    }
}

void Menu::Move_Next() {
    if (menu_options.size() == 0)
        return;
        
    if (selected_option < menu_options.size() - 1)
        selected_option += 1;
}

void Menu::Move_Previous() {
    if (menu_options.size() == 0)
        return;
        
    if (selected_option > 0)
        selected_option -= 1;
}

int Menu::Select() {
    return selected_option;
}

#endif
