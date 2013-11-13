#ifndef MENU_H
#define MENU_H



#include <SDL.h>
#include <vector>

#include "Drawing/TextureManager.h"



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



#endif
