#include "CollisionManager.h"




Collision_Manager * Collision_Manager::instance = NULL;
std::map<std::pair<Collision_Group, Collision_Group>, Collision *> Collision_Manager::collision_pair_map;



Collision_Manager::Collision_Manager() {
	Collidable::Set_Masks();
	//Set Pair Maps
}

Collision_Manager::~Collision_Manager() {
	//Do Stuff
}



void Collision_Manager::Update(double dt) {
	//BROAD PHASE OF COLLISION DETECTION
	//std::deque<Collidable *> possibles;

	for (int i = 0; i < Collidable::objects.size(); i++) {
		//possibles = collidable_tree.Get_Possibles(objects[i]);

		//NARROW PHASE OF COLLISION DETECTION
		for (int j = i + 1; j < Collidable::objects.size(); j++) {
			if (!DoCollide(Collidable::objects[i], Collidable::objects[j]))
				continue;

			if (Collidable::objects[i]->group < Collidable::objects[j]->group)
				collisions.push_back(std::make_pair(Collidable::objects[i], Collidable::objects[j]));
			else
				collisions.push_back(std::make_pair(Collidable::objects[j], Collidable::objects[i]));
		}

		//possibles.clear();
		//END OF NARROW PHASE
	}
	//END OF BROAD PHASE

	//RESOLUTION PHASE OF COLLISION DETECTION
	for (int i = 0; i < collisions.size(); i++)
		if (collision_pair_map[std::make_pair(collisions[i].first->group, collisions[i].second->group)] != NULL)
			collision_pair_map[std::make_pair(collisions[i].first->group, collisions[i].second->group)]->ResolveCollision(collisions[i].first, collisions[i].second);

	collisions.clear();
	//END OF RESOLUTION PHASE
}



Collision_Manager * Collision_Manager::Get_Instance() {
	if (instance == NULL) {
		Set_Collision_Pair_Map();
		instance = new Collision_Manager();
	}
		
	return instance;
}

void Collision_Manager::Delete_Instance() {
	delete instance;
	instance = NULL;
}

void Collision_Manager::Set_Collision_Pair_Map() {
	collision_pair_map[std::make_pair(SHIP_GROUP, SHIP_GROUP)] 			= new ShipToShipCollision();
	collision_pair_map[std::make_pair(SHIP_GROUP, BULLET_GROUP)] 		= new ShipToBulletCollision();
	collision_pair_map[std::make_pair(SHIP_GROUP, PLANET_GROUP)] 		= new ShipToPlanetCollision();
	collision_pair_map[std::make_pair(SHIP_GROUP, MOON_GROUP)] 			= new ShipToMoonCollision();
	collision_pair_map[std::make_pair(SHIP_GROUP, ASTEROID_GROUP)] 		= new ShipToAsteroidCollision();
	collision_pair_map[std::make_pair(SHIP_GROUP, FACTORY_GROUP)] 		= new ShipToFactoryCollision();
	collision_pair_map[std::make_pair(BULLET_GROUP, PLANET_GROUP)] 		= new BulletToPlanetCollision();
	collision_pair_map[std::make_pair(BULLET_GROUP, MOON_GROUP)] 		= new BulletToMoonCollision();
	collision_pair_map[std::make_pair(BULLET_GROUP, ASTEROID_GROUP)] 	= new BulletToAsteroidCollision();
	collision_pair_map[std::make_pair(PLANET_GROUP, ASTEROID_GROUP)] 	= new PlanetToAsteroidCollision();
	collision_pair_map[std::make_pair(MOON_GROUP, ASTEROID_GROUP)] 		= new MoonToAsteroidCollision();
}