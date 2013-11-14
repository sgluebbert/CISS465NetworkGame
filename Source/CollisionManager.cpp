#include "CollisionManager.h"



Collision_Manager * Collision_Manager::instance = NULL;
std::map<std::pair<Collision_Group, Collision_Group>, Collision_Event *> Collision_Manager::collision_pair_map;



Collision_Manager::Collision_Manager() {
	Collidable::Set_Masks();
	//Set Pair Maps
}

Collision_Manager::~Collision_Manager() {
	//Do Stuff
}



void Collision_Manager::Update(double dt) {
	//BROAD PHASE OF COLLISION DETECTION
	std::deque<Collidable *> possibles;

	for (int i = 0; i < Collidable::objects.size(); i++) {
		//possibles = collidable_tree.Get_Possibles(objects[i]);

		//NARROW PHASE OF COLLISION DETECTION
		for (int j = 0; j < possibles.size(); j++) {
			if (&Collidable::objects[i] == &possibles[j])
				continue;

			Collision * temp = NULL;
			
			temp = Collides(Collidable::objects[i], possibles[j]);

			if (temp == NULL)
				continue;

			Collision_Manager::collisions.push_back(temp);
		}

		possibles.clear();
		//END OF NARROW PHASE
	}
	//END OF BROAD PHASE

	//RESOLUTION PHASE OF COLLISION DETECTION
	for (int i = 0; i < Collision_Manager::collisions.size(); i++)
		if (Collision_Manager::collision_pair_map[Collision_Manager::collisions[i]->Get_Pair()] != NULL)
			{}//collision_pair_map[collisions[i]->Get_Pair()]->ResolveCollision(collisions[i]->lhs, collisions[i]->rhs, collisions[i]->x, collisions[i]->y);

	Collision_Manager::collisions.clear();
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