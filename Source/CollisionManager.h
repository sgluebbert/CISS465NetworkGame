#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H



#include <iostream>

#include "Collision.h"



class Collision_Manager {
public:
	void Update(double);

	static Collision_Manager * Get_Instance();
	static void Delete_Instance();

    static void Set_Collision_Pair_Map();
	
private:
    Collision_Manager();
    ~Collision_Manager();

	std::deque< std::pair<Collidable *, Collidable *> > collisions;

	static Collision_Manager * instance;
	static std::map< std::pair<Collision_Group, Collision_Group>, Collision *> collision_pair_map; 
};



#endif