#ifndef COLLIDABLE_H
#define COLLIDABLE_H



#include <map>

#include "Circle.h"



enum Collision_Group {	SHIP_GROUP, BULLET_GROUP, PLANET_GROUP, MOON_GROUP, ASTEROID_GROUP, POWERUP_GROUP	};



class Collidable {
public:
    Circle<float> bounding_volume;
    Collision_Group group;
    int mask;
    
	static std::map<Collision_Group, int> group_masks;
};



#endif