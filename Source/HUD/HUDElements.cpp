#include "HUDElements.h"


/******************************************************************************
 * Base Element methods common to all elements.
 * Hypothetically these should all be pure virtual but ...
 *****************************************************************************/

//-----------------------------------------------------------------------------
// Link, i.e., associate to, a relevant game entity.
// All elements are effectively visitors to entities (double dispatch.)
// Earlier I had a design where HUD Elements were just spying on entities.
// I also tried a design where I had some negotiator but it felt weird.
// And I now think having the entities visit the HUD elements is probably
// a better design. Because the current design doesn't actually offer the
// benefits of the double dispatch that would let us loop over all elements
// and just call the link functions. Since it's the HUD elements that we want
// to loop over (because entities need finer control), it makes more sense.
// In fact, the current design isn't really using the visitor pattern at all
// even though it has the capability to (stuck between two different designs.)
// I removed the corresponding linking functions in entities for now.
// But eh ...
//-----------------------------------------------------------------------------
void HUDElement::link(Map *)
{
}


void HUDElement::link(Client *)
{
}


void HUDElement::link(Entity *)
{
}


void HUDElement::link(Ship *)
{
}


void HUDElement::link(Planet *)
{
}

void HUDElement::link(Moon *)
{
}

void HUDElement::link(Weapon *)
{
}

void HUDElement::link(Pickup *)
{
}


//-----------------------------------------------------------------------------
// Just utilities to link to other elements as necessary.
//-----------------------------------------------------------------------------

// The only one of these that's to be called directly
void HUDElement::linkParent(HUDElement * parentElement)
{
    if (parent == parentElement)
        return;
    
    parent = parentElement;
    
    if (!(parentElement -> active))
        active = false;

    parentElement -> linkChild(this);
}


// Hopefully never called directly
void HUDElement::deLinkParent()
{
    if (parent == NULL) return;
    
    parent -> deLinkChild(this);
    parent = NULL;
    active = false;
}


// Hopefully never called directly
void HUDElement::linkChild(HUDElement * childElement)
{
    if (childElement == NULL) return;
    
    children.push_back(childElement);
    children.unique();

    childElement -> linkParent(this);
}


// Hopefully never called directly
void HUDElement::deLinkChild(HUDElement * childElement)
{
    children.remove(childElement);
    childElement -> deLinkParent();
    childElement -> active = false;
}



// Update data/scaffolding as required
void HUDElement::update()
{
    // Nothing???????
}

// Renderer to draw onto the screen
void HUDElement::draw()
{
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
        it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


// Handle events.
void HUDElement::handleEvent(SDL_Event * event)
{
    // Nothing right now.
}


/******************************************************************************
 * Methods specific to elements of type HUDRoot
 *****************************************************************************/

// Handle events.
void HUDRoot::handleEvent(SDL_Event * event)
{
    // Nothing for now.
}


/******************************************************************************
 * Methods specific to elements of type HUDRadar
 *****************************************************************************/

// Link to a world map.
void HUDRadar::link(Map * _map)
{
    if (_map == NULL || _map == map)
        return;

    map = _map;
    
    // Just decided to use the same width and height for the map instead
    // of trying to resize the radar each time because that looked very messy.
    scale = map -> map_bounds.w / w;   
}


// Render to screen.
void HUDRadar::draw()
{
    float bw = borderWidth;
    
    // Draw border
    DrawRect(x - bw, y - bw, x + w + bw, y + h +  bw, true, borderColor);

    // Draw background
    DrawRect(x, y, x + w, y + h, true, bgColor);
   
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDClientRacon
 *****************************************************************************/

// Link to the client. (This really only uses the ship though?)
void HUDClientRacon::link(Ship * playerShip)
{
    if (playerShip == NULL || playerShip == trackee)
        return;

    trackee = playerShip;
}


// Update the location and state. Always called before drawing.
void HUDClientRacon::update()
{
    // Vestige from an older design that didn't get cleaned up
    HUDRadar * owner = dynamic_cast< HUDRadar * >(parent);

    x = owner -> xOrigin + trackee -> x / owner -> scale;
    y = owner -> yOrigin + trackee -> y / owner -> scale;
}


// Render to the screen.
void HUDClientRacon::draw()
{   
    DrawRect(x - w, y - h, x + w, y + h, true, bodyColor);

    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }    
}


/******************************************************************************
 * Methods specific to elements of type HUDShipRacon
 *****************************************************************************/

// Link to the relevant ship
void HUDShipRacon::link(Ship * ship)
{
    if (ship == NULL || ship == trackee)
        return;

    trackee = ship;
}


// Update the location and state. Always called before drawing.
void HUDShipRacon::update()
{
    // Vestige from an older design that didn't get cleaned up
    HUDRadar * owner = dynamic_cast< HUDRadar * >(parent);

    if (trackee -> team_id == RED_TEAM)
        bodyColor = &RED;
    else if (trackee -> team_id == BLUE_TEAM)
        bodyColor = &BLUE;

    x = owner -> xOrigin + trackee -> x / owner -> scale;
    y = owner -> yOrigin + trackee -> y / owner -> scale;
}


// Render to the screen.
void HUDShipRacon::draw()
{    
    DrawRect(x - w, y - h, x + w, y + h, true, bodyColor);

    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDPlanetRacon
 *****************************************************************************/

// Link to the relevant planet
void HUDPlanetRacon::link(Planet * planet)
{
    if (planet == NULL || planet == trackee)
        return;

    trackee = planet;
    
    // Vestige from an older design that didn't get cleaned up
    HUDRadar * owner = dynamic_cast< HUDRadar * >(parent);

    w = trackee -> drawing_box.w / 2 / owner -> scale;
    h = trackee -> drawing_box.h / 2 / owner -> scale;
}


// Update the location and state. Always called before drawing.
void HUDPlanetRacon::update()
{
    // Vestige from an older design that didn't get cleaned up
    HUDRadar * owner = dynamic_cast< HUDRadar * >(parent);

    x = owner -> xOrigin + trackee -> x / owner -> scale;
    y = owner -> yOrigin + trackee -> y / owner -> scale;
    
    if (trackee -> team_id == RED_TEAM)
        bodyColor = &RED;
    else if (trackee -> team_id == BLUE_TEAM)
        bodyColor = &BLUE;
    else if (trackee -> team_id == NEUTRAL_TEAM)
        bodyColor = &GRAY;
}


// Render to the screen.
void HUDPlanetRacon::draw()
{

//    surface_manager->red_planet->DrawAtRect(x, y, w, h);
    
    DrawCircle(x, y, w / 2, true, bodyColor);

    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}




/******************************************************************************
 * Methods specific to elements of type HUDMoonRacon
 *****************************************************************************/

// Link to the relevant planet
void HUDMoonRacon::link(Moon * moon)
{
    if (moon == NULL || moon == trackee)
        return;

    trackee = moon;
    
    switch (trackee -> type)
    {
        case HEALTH:
            bodyColor = &HEALTH_COLOR;
            break;
        case SHIELD:
            bodyColor = &SHIELD_COLOR;
            break;
        case ARMOR:
            bodyColor = &ARMOR_COLOR;
            break;
        case POWER:
            bodyColor = &POWER_COLOR;
            break;                
    }

    // Vestige from an older design that didn't get cleaned up
    HUDRadar * owner = dynamic_cast< HUDRadar * >(parent);

    w = trackee -> drawing_box.w / 2 / owner -> scale;
    h = trackee -> drawing_box.h / 2 / owner -> scale;
}


// Update the location and state. Always called before drawing.
void HUDMoonRacon::update()
{
    // Vestige from an older design that didn't get cleaned up
    HUDRadar * owner = dynamic_cast< HUDRadar * >(parent);

    x = owner -> xOrigin + trackee -> x / owner -> scale;
    y = owner -> yOrigin + trackee -> y / owner -> scale;
    
    if (trackee -> team_id == RED_TEAM)
        borderColor = &RED;
    else if (trackee -> team_id == BLUE_TEAM)
        borderColor = &BLUE;
    else if (trackee -> team_id == NEUTRAL_TEAM)
        borderColor = &GRAY;
}


// Render to the screen.
void HUDMoonRacon::draw()
{

    std::cout << "x, y, w: " << x << ", " << y << ", " << w << std::endl;
//    surface_manager->red_planet->DrawAtRect(x, y, w, h);
    
    DrawCircle(x, y, w / 2, true, bodyColor);

    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}




/******************************************************************************
 * Methods specific to elements of type HUDHullBar
 *****************************************************************************/

// Link to the relevant ship
void HUDHullBar::link(Ship * ship)
{
    if (ship == NULL || ship == linkee)
        return;

    linkee = ship;
}


// Update the location and state. Always called before drawing.
void HUDHullBar::update()
{
    healthRatio = linkee -> health / linkee -> max_health;
}


// Render to the screen.
void HUDHullBar::draw()
{
    float bw = borderWidth;
    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w * healthRatio, y + h, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDShieldBar
 *****************************************************************************/

// Link to the relevant ship
void HUDShieldBar::link(Ship * ship)
{
    if (ship == NULL || ship == linkee)
        return;

    linkee = ship;
}


// Update the location and state. Always called before drawing.
void HUDShieldBar::update()
{
    shieldRatio = linkee -> shields / linkee -> max_shields;
}


// Render to the screen.
void HUDShieldBar::draw()
{
    float bw = borderWidth;
    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w * shieldRatio, y + h, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDArmorBar
 *****************************************************************************/

// Link to the relevant ship
void HUDArmorBar::link(Ship * ship)
{
    if (ship == NULL || ship == linkee)
        return;

    linkee = ship;
}


// Update the location and state. Always called before drawing.
void HUDArmorBar::update()
{
    armorRatio = linkee -> armor / linkee -> max_armor;
}


// Render to the screen.
void HUDArmorBar::draw()
{
    float bw = borderWidth;
    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w * armorRatio, y + h, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDPowerBar
 *****************************************************************************/

// Link to the relevant ship
void HUDPowerBar::link(Ship * ship)
{
    if (ship == NULL || ship == linkee)
        return;

    linkee = ship;
}


// Update the location and state. Always called before drawing.
void HUDPowerBar::update()
{
    powerRatio = linkee -> power / linkee -> max_power;
}


// Render to the screen.
void HUDPowerBar::draw()
{
    float bw = borderWidth;
    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w * powerRatio, y + h, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDWeaponBay
 *****************************************************************************/

// Render to the screen.
void HUDWeaponBay::draw()
{
    float bw = borderWidth;

    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);
    
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDLaserCD
 *****************************************************************************/

// Link to the relevant ship
void HUDLaserCD::link(Weapon * weapon)
{
    if (weapon == NULL || weapon == linkee)
        return;

    linkee = weapon;
}


// Update the location and state. Always called before drawing.
void HUDLaserCD::update()
{
    if (linkee -> state == WEAPON_FIRING || linkee -> state == WEAPON_READY)
        timeRatio = 1.0;
    else
        timeRatio = linkee -> recharge_timer.Get_Progress();

    // Vestige from an older design that didn't get cleaned up
    HUDWeaponBay * owner = dynamic_cast< HUDWeaponBay * >(parent);

    w = owner -> w / 6;
    h = owner -> h / 1.1;
    x = owner -> x + owner -> borderWidth + 12 * borderWidth;
    y = owner -> y + owner -> borderWidth + 2 * borderWidth;
}


// Render to the screen.
void HUDLaserCD::draw()
{
    float bw = borderWidth;
    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w, y + h * timeRatio, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDGaussCD
 *****************************************************************************/

// Link to the relevant ship
void HUDGaussCD::link(Weapon * weapon)
{
    if (weapon == NULL || weapon == linkee)
        return;

    linkee = weapon;
}


// Update the location and state. Always called before drawing.
void HUDGaussCD::update()
{
    if (linkee -> state == WEAPON_FIRING || linkee -> state == WEAPON_READY)
        timeRatio = 1.0;
    else
        timeRatio = linkee -> recharge_timer.Get_Progress();

    // Vestige from an older design that didn't get cleaned up
    HUDWeaponBay * owner = dynamic_cast< HUDWeaponBay * >(parent);

    w = owner -> w / 6;
    h = owner -> h / 1.1;
    x = owner -> x + owner -> borderWidth + 24 * borderWidth + w;
    y = owner -> y + owner -> borderWidth + 2 * borderWidth;
}


// Render to the screen.
void HUDGaussCD::draw()
{
    float bw = borderWidth;
    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w, y + h * timeRatio, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDRocketCD
 *****************************************************************************/

// Link to the relevant ship
void HUDRocketCD::link(Weapon * weapon)
{
    if (weapon == NULL || weapon == linkee)
        return;

    linkee = weapon;
}


// Update the location and state. Always called before drawing.
void HUDRocketCD::update()
{
    if (linkee -> state == WEAPON_FIRING || linkee -> state == WEAPON_READY)
        timeRatio = 1.0;
    else
        timeRatio = linkee -> recharge_timer.Get_Progress();

    // Vestige from an older design that didn't get cleaned up
    HUDWeaponBay * owner = dynamic_cast< HUDWeaponBay * >(parent);

    w = owner -> w / 6;
    h = owner -> h / 1.1;
    x = owner -> x + owner -> borderWidth + 36 * borderWidth + 2 * w;
    y = owner -> y + owner -> borderWidth + 2 * borderWidth;
}


// Render to the screen.
void HUDRocketCD::draw()
{
    float bw = borderWidth;
    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w, y + h * timeRatio, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDBombCD
 *****************************************************************************/

// Link to the relevant ship
void HUDBombCD::link(Weapon * weapon)
{
    if (weapon == NULL || weapon == linkee)
        return;

    linkee = weapon;
}


// Update the location and state. Always called before drawing.
void HUDBombCD::update()
{
    if (linkee -> state == WEAPON_FIRING || linkee -> state == WEAPON_READY)
        timeRatio = 1.0;
    else
        timeRatio = linkee -> recharge_timer.Get_Progress();

    // Vestige from an older design that didn't get cleaned up
    HUDWeaponBay * owner = dynamic_cast< HUDWeaponBay * >(parent);

    w = owner -> w / 6;
    h = owner -> h / 1.1;
    x = owner -> x + owner -> borderWidth + 48 * borderWidth + 3 * w;
    y = owner -> y + owner -> borderWidth + 2 * borderWidth;
}


// Render to the screen.
void HUDBombCD::draw()
{
    float bw = borderWidth;
    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w, y + h * timeRatio, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDPCaptureDisp
 *****************************************************************************/

// Link to the relevant ship
void HUDPCaptureDisp::link(Planet * planet)
{
    if (planet == NULL || planet == linkee)
        return;

    linkee = planet;
}


// Update the location and state. Always called before drawing.
void HUDPCaptureDisp::update()
{
    if (!parent -> active)
        return;
    
    if (linkee -> locked)
        active = false;
    else
        active = true;
    
    alignment = linkee -> alignment;
    if (alignment > 0)
        fgColor = &BLUE;
    else if (alignment < 0)
        fgColor = &RED;
}


// Render to the screen.
void HUDPCaptureDisp::draw()
{
    float absAlignment = alignment;
    if (absAlignment < 0)
        absAlignment *= -1.0;

    float bw = borderWidth;

    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w * absAlignment, y + h, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}


/******************************************************************************
 * Methods specific to elements of type HUDMCaptureDisp
 *****************************************************************************/

// Link to the relevant ship
void HUDMCaptureDisp::link(Moon * moon)
{
    if (moon == NULL || moon == linkee)
        return;

    linkee = moon;
}


// Update the location and state. Always called before drawing.
void HUDMCaptureDisp::update()
{
    if (!parent -> active)
        return;
    
    if (linkee -> alignment == alignment)
        active = false;
    else
        active = true;
    
    alignment = linkee -> alignment;
    if (alignment > 0)
        fgColor = &BLUE;
    else if (alignment < 0)
        fgColor = &RED;
}


// Render to the screen.
void HUDMCaptureDisp::draw()
{
    float absAlignment = alignment;
    if (absAlignment < 0)
        absAlignment *= -1.0;

    float bw = borderWidth;

    DrawRect(x - bw, y - bw, x + w + bw, y + h + bw, true, borderColor); 
    
    DrawRect(x, y, x + w, y + h, true, bgColor);

    DrawRect(x, y, x + w * absAlignment, y + h, true, fgColor);
    
    // Presumably no children unless we support fancy stuff like nametags
    if (children.empty())
        return;

    for (std::list< HUDElement * >::iterator it = children.begin();
         it != children.end(); ++it)
    {
        if ((*it) -> active)
            (*it) -> draw();
    }
}




