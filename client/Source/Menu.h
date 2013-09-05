#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <vector>

//#include "Font.h"
#include "Surface.h"

class Menu {
public:
    Menu();
    Menu(std::string);
    Menu(std::string[], int);
    
    void AddOption(std::string);
    void SetPosition(int, int);
    void SetSize(int, int);
    void SetOffset(int, int);
    void SetOrientation(bool);
    void SetBackground(SDL_Surface *);
    
    void Draw(SDL_Surface *);
        
    void Move_Next();
    void Move_Previous();
    virtual void Select();
    
protected:
    std::vector<std::string> menu_options;
    int selected_option;
    
    int menu_x, menu_y;
    int margin_x, margin_y;
    int offset_x, offset_y;
    int option_width, option_height;
    
    bool vertically_oriented;
    
    SDL_Surface * option_background;
    //Font menu_font;
};

Menu::Menu() {
    selected_option = -1;
    menu_x = 0;
    menu_y = 0;
    offset_x = 0;
    offset_y = 0;
    option_width = 100;
    option_height = 50;
    vertically_oriented = true;
    option_background = NULL;
}

Menu::Menu(std::string newOption) {
    menu_options.push_back(newOption);
    selected_option = 0;
    menu_x = 0;
    menu_y = 0;
    offset_x = 0;
    offset_y = 0;
    option_width = 100;
    option_height = 50;
    vertically_oriented = true;
    option_background = NULL;
}

Menu::Menu(std::string * newOptions, int size) {
    for (int i = 0; i < size; i++)
        menu_options.push_back(newOptions[i]);
    selected_option = 0;
    menu_x = 0;
    menu_y = 0;
    offset_x = 0;
    offset_y = 0;
    option_width = 100;
    option_height = 50;
    vertically_oriented = true;
    option_background = NULL;
}

void Menu::AddOption(std::string newOption) {
    menu_options.push_back(newOption);
}

void Menu::SetPosition(int newX, int newY) {
    menu_x = newX;
    menu_y = newY;
}

void Menu::SetSize(int newW, int newH) {
    option_width = newW;
    option_height = newH;
}

void Menu::SetOffset(int newoffset_x, int newoffset_y) {
    offset_x = newoffset_x;
    offset_y = newoffset_y;
}

void Menu::SetBackground(SDL_Surface * SurfSrc) {
    if (option_background != NULL)
        delete option_background;
        
    option_background = SurfSrc;
}

void Menu::SetOrientation(bool newFlag) {
    vertically_oriented = newFlag;
}

void Menu::Draw(SDL_Surface * Surf_Dest) {
    int tempX = menu_x;
    int tempY = menu_y;
    
    for (int i = 0; i < menu_options.size(); i++) {
        Surface::Blit(Surf_Dest, option_background, 0, 0, tempX, tempY, option_width, option_height);
        //Draw Text
        
        if (vertically_oriented) {
            tempX += offset_x;
            tempY += (option_height + margin_y);
        }
        else {
            tempX += (option_width + margin_x);
            tempY += offset_y;
        }
    }
}

void Menu::Move_Next() {
    if (selected_option <= -1)
        return;
        
    if (selected_option < menu_options.size() - 1)
        selected_option += 1;
}

void Menu::Move_Previous() {
    if (selected_option <= -1)
        return;
        
    if (selected_option > 0)
        selected_option -= 1;
}

void Menu::Select() {
    switch(selected_option) {
    default:
        return;
    }
}

#endif
