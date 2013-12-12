#ifndef HUDMANAGER_H
#define HUDMANAGER_H


#include "HUDElements.h"


class HUDManager
{
public:
    static void cleanup();

    static void update();
    
    static void draw();

    static void handleEvent(SDL_Event * event);

    static HUDElement * createHUDElement(HUDElementType);

    static void removeHUDElement(HUDElement *);

    static std::list< HUDElement * > allHUDElements;
    static std::list< HUDRoot * > allHUDRoots;
};


#endif
