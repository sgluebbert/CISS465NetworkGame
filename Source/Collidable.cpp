#include "Collidable.h"



std::deque<Collidable *> Collidable::objects;
std::map<Collision_Group, std::bitset<8> > Collidable::group_masks;



void Collidable::Set_Group(Collision_Group new_group) {
	group = new_group;
	mask = Collidable::group_masks[group];
}



void Collidable::Set_Masks() {
	std::bitset<8> temp_mask;

	temp_mask[NO_GROUP]				= false;
	temp_mask[SHIP_GROUP]			= false;
	temp_mask[BULLET_GROUP]			= false;
	temp_mask[PLANET_GROUP]			= false;
	temp_mask[MOON_GROUP]			= false;
	temp_mask[ASTEROID_GROUP]		= false;
	temp_mask[FACTORY_GROUP]		= false;
	group_masks[NO_GROUP] 			= temp_mask;

	temp_mask[NO_GROUP]				= false;
	temp_mask[SHIP_GROUP]			= true;
	temp_mask[BULLET_GROUP]			= true;
	temp_mask[PLANET_GROUP]			= true;
	temp_mask[MOON_GROUP]			= true;
	temp_mask[ASTEROID_GROUP]		= true;
	temp_mask[FACTORY_GROUP]		= true;
	group_masks[SHIP_GROUP] 		= temp_mask;

	temp_mask[NO_GROUP]				= false;
	temp_mask[SHIP_GROUP]			= true;
	temp_mask[BULLET_GROUP]			= false;
	temp_mask[PLANET_GROUP]			= true;
	temp_mask[MOON_GROUP]			= true;
	temp_mask[ASTEROID_GROUP]		= true;
	temp_mask[FACTORY_GROUP]		= false;
	group_masks[BULLET_GROUP] 		= temp_mask;

	temp_mask[NO_GROUP]				= false;
	temp_mask[SHIP_GROUP]			= true;
	temp_mask[BULLET_GROUP]			= true;
	temp_mask[PLANET_GROUP]			= false;
	temp_mask[MOON_GROUP]			= false;
	temp_mask[ASTEROID_GROUP]		= true;
	temp_mask[FACTORY_GROUP]		= false;
	group_masks[PLANET_GROUP] 		= temp_mask;

	temp_mask[NO_GROUP]				= false;
	temp_mask[SHIP_GROUP]			= true;
	temp_mask[BULLET_GROUP]			= true;
	temp_mask[PLANET_GROUP]			= true;
	temp_mask[MOON_GROUP]			= true;
	temp_mask[ASTEROID_GROUP]		= true;
	temp_mask[FACTORY_GROUP]		= false;
	group_masks[MOON_GROUP] 		= temp_mask;

	temp_mask[NO_GROUP]				= false;
	temp_mask[SHIP_GROUP]			= true;
	temp_mask[BULLET_GROUP]			= true;
	temp_mask[PLANET_GROUP]			= true;
	temp_mask[MOON_GROUP]			= true;
	temp_mask[ASTEROID_GROUP]		= true;
	temp_mask[FACTORY_GROUP]		= false;
	group_masks[ASTEROID_GROUP] 	= temp_mask;

	temp_mask[NO_GROUP]				= false;
	temp_mask[SHIP_GROUP]			= true;
	temp_mask[BULLET_GROUP]			= false;
	temp_mask[PLANET_GROUP]			= false;
	temp_mask[MOON_GROUP]			= false;
	temp_mask[ASTEROID_GROUP]		= false;
	temp_mask[FACTORY_GROUP]		= false;
	group_masks[FACTORY_GROUP] 		= temp_mask;
}