#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H



#include "Collision.h"



class Collision_Manager {
public:
	void Update(double);

	static Collision_Manager * Get_Instance();
	static void Delete_Instance();
	
private:
    Collision_Manager();
    ~Collision_Manager();

	std::deque<Collision *> collisions;

	static Collision_Manager * instance;
	static std::map<std::pair<Collision_Group, Collision_Group>, Collision_Event *> collision_pair_map; 
};



#endif