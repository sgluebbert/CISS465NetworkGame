#ifndef COLLIDABLE_H
#define COLLIDABLE_H



#include <bitset>
#include <deque>
#include <map>

#include "Circle.h"



enum Collision_Group {	NO_GROUP, SHIP_GROUP, BULLET_GROUP, PLANET_GROUP, MOON_GROUP, ASTEROID_GROUP, FACTORY_GROUP		};



class Collidable {
public:
	void Set_Group(Collision_Group);

    Circle<float> bounding_volume;
    Collision_Group group;
    std::bitset<8> mask;

    static void Set_Masks();
    
	static std::deque<Collidable *> objects;
	static std::map<Collision_Group, std::bitset<8> > group_masks;
};



#endif