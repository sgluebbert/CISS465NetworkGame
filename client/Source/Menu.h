#ifndef MENU_H
#define MENU_H



#include <SDL.h>
#include <vector>

#include "TextureManager.h"



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
    void Set_Background(Texture *);
    
    void Draw();
        
    void Move_Next();
    void Move_Previous();
    int Select();
    
private:
    std::vector<std::string> menu_options;
    int selected_option;
    
    int menu_x, menu_y;
    int margin_x, margin_y;
    int offset_x, offset_y;
    
    bool vertically_oriented;
    
    Texture *option_background;
    SDL_Rect option_rect;
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

void Menu::Set_Background(Texture *texture) {
    option_background = texture;
}

void Menu::Set_Orientation(bool newFlag) {
    vertically_oriented = newFlag;
}

void Menu::Draw() {
    // SDL_Surface * temp_surf = NULL;
    SDL_Rect temp_rect;
    int tempX = menu_x;
    int tempY = menu_y;
    
    for (int i = 0; i < menu_options.size(); i++) {
        option_rect.x = tempX;
        option_rect.y = tempY;
        
        if (i == selected_option)
            option_background = surface_manager->highlightedbutton;
        else
            option_background = surface_manager->button;
        
        option_background->DrawAtRect(option_rect.x, option_rect.y);

        Text temp(menu_options[i].c_str(), TextureManager::GetInstance()->fonts.font_FreeMono_16, BLACK);
        temp_rect.x = (option_rect.w - temp.width) / 2.0 + option_rect.x;
        temp_rect.y = (option_rect.h - temp.height) / 2.0 + option_rect.y;
        temp.Draw(temp_rect.x, temp_rect.y);

        glColor4f(1, 1, 1, 1);
        
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
