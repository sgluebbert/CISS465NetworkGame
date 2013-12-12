#ifndef HUDELMENTS_H
#define HUDELEMENTS_H


#include "HUDincludes.h"


// List of all the different types of HUDs that will be concretely active.
enum HUDElementType
{
    HUD_ROOT, // Base context for a set of HUDs.
    HUD_RADAR, // Minimap
    HUD_CLIENT_RACON, // Representation of the client on the minimap
    HUD_SHIP_RACON, // Representation of a ship on the minimap
    HUD_PLANET_RACON, // Representation of a planet on the minimap
    HUD_MOON_RACON, // Representation of a moon on the minimap
    HUD_PICKUP_RACON, // Representation of a pickup on the minimap
    HUD_HULL_BAR, // Shows the status of the Hull of the player ship
    HUD_SHIELD_BAR, // Shows the status of the Shield of the player ship
    HUD_ARMOR_BAR, // Shows the status of the Armor of the player ship
    HUD_POWER_BAR, // Shows the status of the Power of the player ship
    HUD_WEAPON_BAY, // Context for the weapon HUDs of a ship
    HUD_LASER_CD, // Shows the status of the energy weapon of a ship
    HUD_GAUSS_CD, // Shows the status of the ballistic weapon of a ship
    HUD_ROCKET_CD, // Shows the status of the propelled weapon of a ship
    HUD_BOMB_CD, // Shows the status of the bomb of a ship
    HUD_PCAPTURE_DISP, // Displays the status of an ongoing planet capture
    HUD_MCAPTURE_DISP, // Displays the status of an ongoing moon capture
};


// The base class for all HUD elements.
class HUDElement
{
public:
    virtual ~HUDElement()
    {
        // Just for ceremonial purposes???
    }

    // In most cases, only called before draw.
    virtual void update();
    
    // Render to the screen as relevant (also implicitly updates mostly.)
    virtual void draw();

    // These just associate the HUD element with respective game entities.
    virtual void link(Map *);
    virtual void link(Client *); // not really used????
    virtual void link(Entity *); // not really used ???
    virtual void link(Ship *);
    virtual void link(Planet *);
    virtual void link(Moon *);
    virtual void link(Weapon *); // not really used???
    virtual void link(Pickup *);

    // Handle any event propagated to the element.
    virtual void handleEvent(SDL_Event *);

    // Just utilities to link to other elements as necessary.
    virtual void linkParent(HUDElement *);
    virtual void deLinkParent();
    virtual void linkChild(HUDElement *);
    virtual void deLinkChild(HUDElement *);

    // These should be private but ...
// private:

    // Just the HUD element that this particular element is on top of.
    HUDElement * parent;

    // Just a list of all elements that are immediately on top of this element.
    std::list< HUDElement * > children;
    
    // Whether the HUD Element is active
    bool active;

    // Location on screen.
    float x, y, w, h;

    // Type of the element.
    HUDElementType type;
};

// Sets the context for a set of HUD elements.
// All the HUDs together in a set are essentially treated as a common unit.
class HUDRoot : public HUDElement
{
public:
    HUDRoot( float _x=0,
             float _y=0,
             float _w=WINDOW_BOUNDING_BOX.w,
             float _h=WINDOW_BOUNDING_BOX.h)
    {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
        active = true;
        type = HUD_ROOT;
    }

    void handleEvent(SDL_Event *);
};


// This is a radar (minimap) that tracks any relevant game entities.
class HUDRadar : public HUDElement
{
public:
    HUDRadar(float _x=0,
             float _y=WINDOW_BOUNDING_BOX.h - WINDOW_BOUNDING_BOX.w / 6,
             float _w=WINDOW_BOUNDING_BOX.w / 6,
             float _h=WINDOW_BOUNDING_BOX.w / 6,
             float _bw=2,
             const Color * _bgColor=&BLACK,
             const Color * _borderColor=&RADAR_BORDER)
    {
        active = true;
        type = HUD_RADAR;
        x = _x + _bw;
        y = _y - _bw;
        w = _w;
        h = _h;
        xOrigin = x + w / 2.0f;
        yOrigin = y + h / 2.0f;
        borderWidth = _bw;
        bgColor = _bgColor;
        borderColor = _borderColor;
    }

    // Link to a world map
    void link(Map *);

//     // update the link to the map in case of a change
//     void update();
    
    // Render onto screen
    void draw();

    // Width of the decorative border
    float borderWidth;

    // Scale to the world map
    float scale;

    // Offsets to mirror the world map co-ordinate system
    float xOrigin, yOrigin;
    
    // Colors of the backdrop and the border
    const Color * bgColor;
    const Color * borderColor;

    Map * map;
};


// These are the different types of Radar Beacons (Racons)
// Could probably use an intermediate Racon class
// Or make the HUD elements the static listeners and the entities the visitors
class HUDClientRacon : public HUDElement
{
public:
    HUDClientRacon(const Color * _bodyColor=&WHITE)
    {
        active = true;
        type = HUD_CLIENT_RACON;
        bodyColor = _bodyColor;
        w = 2; // Too small otherwise imo 
        h = 2;
    }

    // We actually just use a ship, no?????
    void link(Ship *);

    void update();

    void draw();

    const Color * bodyColor;

    // The tracked ship
    Ship * trackee;
};


class HUDShipRacon : public HUDElement
{
public:
    HUDShipRacon(const Color * _bodyColor=&GRAY)
    {
        active = true;
        type = HUD_SHIP_RACON;
        bodyColor = _bodyColor;
        w = 2; // Too small otherwise imo 
        h = 2;
    }

    void link(Ship *);

    void update();

    void draw();

    const Color * bodyColor;

    // The tracked ship
    Ship * trackee;
};


class HUDPlanetRacon : public HUDElement
{
public:
    HUDPlanetRacon(const Color * _bodyColor=&GRAY)
    {
        active = true;
        type = HUD_PLANET_RACON;
        bodyColor = _bodyColor;
    }

    void link(Planet *);

    void update();

    void draw();

    const Color * bodyColor;

    // The tracked ship
    Planet * trackee;
};


class HUDMoonRacon : public HUDElement
{
public:
    HUDMoonRacon(const Color * _borderColor=&GRAY)
    {
        active = true;
        type = HUD_MOON_RACON;
        borderColor = _borderColor;
    }

    void link(Moon *);

    void update();

    void draw();

    const Color * borderColor;
    const Color * bodyColor;

    // The tracked ship
    Moon * trackee;
};


class HUDHullBar : public HUDElement
{
public:
    HUDHullBar(float _x=0,
               float _y=WINDOW_BOUNDING_BOX.h / 32 + 10,
               float _w=WINDOW_BOUNDING_BOX.w / 8,
               float _h=WINDOW_BOUNDING_BOX.h / 32,
               float _bw=1,
               const Color * _borderColor=&WHITE,
               const Color * _bgColor=&BLACK,
               const Color * _fgColor=&HEALTH_COLOR)
    {
        active = true;
        type = HUD_HULL_BAR;
        x = _x + _bw;
        y = _y + _bw;
        w = _w;
        h = _h;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Ship *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked ship (is linkee even a word?)
    Ship * linkee;

    // The ratio of current health to maximum health
    float healthRatio;
};


class HUDShieldBar : public HUDElement
{
public:
    HUDShieldBar(float _x=0,
                 float _y=0,
                 float _w=WINDOW_BOUNDING_BOX.w / 8,
                 float _h=WINDOW_BOUNDING_BOX.h / 32,
                 float _bw=1,
                 const Color * _borderColor=&WHITE,
                 const Color * _bgColor=&GRAY,
                 const Color * _fgColor=&SHIELD_COLOR)
    {
        active = true;
        type = HUD_SHIELD_BAR;
        x = _x + _bw;
        y = _y + _bw;
        w = _w;
        h = _h;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Ship *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked ship
    Ship * linkee;

    // The ratio of current shields to maximum shields
    float shieldRatio;
};


class HUDArmorBar : public HUDElement
{
public:
    HUDArmorBar(float _x=WINDOW_BOUNDING_BOX.w - WINDOW_BOUNDING_BOX.w / 8 - 2,
                float _y=0,
                float _w=WINDOW_BOUNDING_BOX.w / 8,
                float _h=WINDOW_BOUNDING_BOX.h / 32,
                float _bw=1,
                const Color * _borderColor=&WHITE,
                const Color * _bgColor=&GRAY,
                const Color * _fgColor=&ARMOR_COLOR)
    {
        active = true;
        type = HUD_ARMOR_BAR;
        x = _x + _bw;
        y = _y + _bw;
        w = _w;
        h = _h;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Ship *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked ship
    Ship * linkee;

    // The ratio of current armor to maximum armor
    float armorRatio;
};


class HUDPowerBar : public HUDElement
{
public:
    HUDPowerBar(float _x=WINDOW_BOUNDING_BOX.w - WINDOW_BOUNDING_BOX.w / 8 - 2,
                float _y=WINDOW_BOUNDING_BOX.h / 32 + 10,
                float _w=WINDOW_BOUNDING_BOX.w / 8,
                float _h=WINDOW_BOUNDING_BOX.h / 32,
                float _bw=1,
                const Color * _borderColor=&WHITE,
                const Color * _bgColor=&BLACK,
                const Color * _fgColor=&POWER_COLOR)
    {
        active = true;
        type = HUD_POWER_BAR;
        x = _x + _bw;
        y = _y + _bw;
        w = _w;
        h = _h;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Ship *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked ship
    Ship * linkee;

    // The ratio of current power to maximum power
    float powerRatio;
};


// Context for weapon HUDs
class HUDWeaponBay : public HUDElement
{
public:
    HUDWeaponBay(float _x=WINDOW_BOUNDING_BOX.w / 2 - WINDOW_BOUNDING_BOX.w / 8,
                 float _y=WINDOW_BOUNDING_BOX.h - WINDOW_BOUNDING_BOX.w / 12,
                 float _w=WINDOW_BOUNDING_BOX.w / 4,
                 float _h=WINDOW_BOUNDING_BOX.w / 12,
                 float _bw=2,
                 const Color * _bgColor=&WEAPONBAY_BODY,
                 const Color * _borderColor=&WEAPONBAY_BORDER)
    {
        active = true;
        type = HUD_WEAPON_BAY;
        x = _x + _bw;
        y = _y - _bw;
        w = _w;
        h = _h;
        borderWidth = _bw;
        bgColor = _bgColor;
        borderColor = _borderColor;
    }
    
    // Render onto screen
    void draw();

    // Width of the decorative border
    float borderWidth;
    
    // Colors of the backdrop and the border
    const Color * bgColor;
    const Color * borderColor;
};


// Shows status of Energy type weapon of a ship
class HUDLaserCD : public HUDElement
{
public:
    HUDLaserCD(float _bw = 1,
               const Color * _borderColor=&WHITE,
               const Color * _bgColor=&GRAY,
               const Color * _fgColor=&WEAPONCD)
    {
        active = true;
        type = HUD_LASER_CD;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Weapon *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked weapon
    Weapon * linkee;

    // The ratio of time elapsed to total time for cooldown.
    float timeRatio;
};


// Shows status of Ballistic type weapon of a ship
class HUDGaussCD : public HUDElement
{
public:
    HUDGaussCD(float _bw = 1,
               const Color * _borderColor=&WHITE,
               const Color * _bgColor=&GRAY,
               const Color * _fgColor=&WEAPONCD)
    {
        active = true;
        type = HUD_GAUSS_CD;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Weapon *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked weapon
    Weapon * linkee;

    // The ratio of time elapsed to total time for cooldown.
    float timeRatio;
};


// Shows status of propelled type weapon of a ship
class HUDRocketCD : public HUDElement
{
public:
    HUDRocketCD(float _bw = 1,
               const Color * _borderColor=&WHITE,
               const Color * _bgColor=&GRAY,
               const Color * _fgColor=&WEAPONCD)
    {
        active = true;
        type = HUD_ROCKET_CD;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Weapon *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked weapon
    Weapon * linkee;

    // The ratio of time elapsed to total time for cooldown.
    float timeRatio;
};


// Shows status of bomb type weapon of a ship
class HUDBombCD : public HUDElement
{
public:
    HUDBombCD(float _bw = 1,
               const Color * _borderColor=&WHITE,
               const Color * _bgColor=&GRAY,
               const Color * _fgColor=&WEAPONCD)
    {
        active = true;
        type = HUD_BOMB_CD;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Weapon *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked weapon
    Weapon * linkee;

    // The ratio of time elapsed to total time for cooldown.
    float timeRatio;
};


// Displays the status of an ongoing planet capture
class HUDPCaptureDisp : public HUDElement
{
public:
    HUDPCaptureDisp(float _x=WINDOW_BOUNDING_BOX.w - WINDOW_BOUNDING_BOX.w / 8 - 2,
                    float _y=WINDOW_BOUNDING_BOX.h - WINDOW_BOUNDING_BOX.h / 32 - 2,
                    float _w=WINDOW_BOUNDING_BOX.w / 8,
                    float _h=WINDOW_BOUNDING_BOX.h / 32,
                    float _bw=1,
                    const Color * _borderColor=&WHITE,
                    const Color * _bgColor=&GRAY,
                    const Color * _fgColor=&GRAY)
    {
        type = HUD_PCAPTURE_DISP;
        x = _x + _bw;
        y = _y + _bw;
        w = _w;
        h = _h;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Planet *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked planet
    Planet * linkee;

    // The alignment of the planet
    float alignment;
};


// Displays the status of an ongoing moon capture
class HUDMCaptureDisp : public HUDElement
{
public:
    HUDMCaptureDisp(float _x=0,
                    float _y=WINDOW_BOUNDING_BOX.h / 32 + 20,
                    float _w=WINDOW_BOUNDING_BOX.w / 8,
                    float _h=WINDOW_BOUNDING_BOX.h / 32,
                    float _bw=1,
                    const Color * _borderColor=&WHITE,
                    const Color * _bgColor=&GRAY,
                    const Color * _fgColor=&GRAY)
    {
        type = HUD_MCAPTURE_DISP;
        x = _x + _bw;
        y = _y + _bw;
        w = _w;
        h = _h;
        borderWidth = _bw;
        borderColor = _borderColor;
        bgColor = _bgColor;
        fgColor = _fgColor;
    }

    void link(Moon *);

    void update();
    
    void draw();

    float borderWidth;

    const Color * borderColor;
    const Color * bgColor;
    const Color * fgColor;

    // The linked planet
    Moon * linkee;

    // The alignment of the planet
    float alignment;
};


#endif
