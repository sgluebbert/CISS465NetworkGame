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
		for (int j = 0; j < Collidable::objects.size(); j++) {
			if (i == j)
				continue;
			
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
			std::cout << "COLLISION!" << std::endl;//collision_pair_map[collisions[i]->Get_Pair()]->ResolveCollision(collisions[i]->lhs, collisions[i]->rhs, collisions[i]->x, collisions[i]->y);

	collisions.clear();
	//END OF RESOLUTION PHASE
}



Collision_Manager * Collision_Manager::Get_Instance() {
	if (instance == NULL)
		instance = new Collision_Manager();
		
	return instance;
}

void Collision_Manager::Delete_Instance() {
	delete instance;
	instance = NULL;
}