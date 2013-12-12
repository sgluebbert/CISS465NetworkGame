#include "HUDManager.h"


std::list< HUDElement * > HUDManager::allHUDElements;
std::list< HUDRoot * > HUDManager::allHUDRoots;


// Not so graceful cleaning up
void HUDManager::cleanup()
{
    for (std::list< HUDElement * >::iterator it = allHUDElements.begin();
         it != allHUDElements.end(); ++it)
        delete *it;

    allHUDElements.clear();
    allHUDRoots.clear();
}


// Updates stuff that need to be called outside of draw.
void HUDManager::update()
{
    if (allHUDElements.empty()) return;
    
    for(std::list< HUDElement * >::iterator it = allHUDElements.begin();
        it != allHUDElements.end(); ++it)
    {
        (*it) -> update();
    }
}
    

// Draws the relevant HUD
void HUDManager::draw()
{
    if (allHUDRoots.empty()) return;

    for(std::list< HUDRoot * >::iterator it = allHUDRoots.begin();
        it != allHUDRoots.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }    
}


// Uhh some other time.
void HUDManager::handleEvent(SDL_Event * event)
{
}


HUDElement * HUDManager::createHUDElement(HUDElementType type)
{
    HUDElement * ret;
    switch(type)
    {
        case HUD_ROOT:
            ret = new HUDRoot();
            allHUDRoots.push_back(dynamic_cast< HUDRoot * >(ret));
            break;
        case HUD_RADAR:
            ret = new HUDRadar();
            break;
        case HUD_CLIENT_RACON:
            ret = new HUDClientRacon();
            break;
        case HUD_SHIP_RACON:
            ret = new HUDShipRacon();
            break;
        case HUD_PLANET_RACON:
            ret = new HUDPlanetRacon();
            break;
        case HUD_MOON_RACON:
            ret = new HUDMoonRacon();
            break;
        case HUD_HULL_BAR:
            ret = new HUDHullBar();
            break;
        case HUD_SHIELD_BAR:
            ret = new HUDShieldBar();
            break;
        case HUD_ARMOR_BAR:
            ret = new HUDArmorBar();
            break;
        case HUD_POWER_BAR:
            ret = new HUDPowerBar();
            break;
        case HUD_WEAPON_BAY:
            ret = new HUDWeaponBay();
            break;
        case HUD_LASER_CD:
            ret = new HUDLaserCD();
            break;
        case HUD_GAUSS_CD:
            ret = new HUDGaussCD();
            break;
        case HUD_ROCKET_CD:
            ret = new HUDRocketCD();
            break;
        case HUD_BOMB_CD:
            ret = new HUDBombCD();
            break;
        case HUD_PCAPTURE_DISP:
            ret = new HUDPCaptureDisp();
            break;
        case HUD_MCAPTURE_DISP:
            ret = new HUDMCaptureDisp();
            break;

    }

    allHUDElements.push_back(ret);

    return ret;
}


void HUDManager::removeHUDElement(HUDElement * elem)
{
    if (elem == NULL)
        return;

    elem -> deLinkParent();

    if (elem -> children.empty());
    else
    {
        for(std::list<HUDElement *>::iterator it = elem -> children.begin();
            it != elem -> children.end(); ++it)
        {
            if ((*it) -> active)
                (*it) -> active = false;
            (*it) -> deLinkParent();
        }
    }
    
    if (elem -> type == HUD_ROOT)
        allHUDRoots.remove(dynamic_cast< HUDRoot * >(elem));
    allHUDElements.remove(elem);

    delete elem;
}
