#include "Collision.h"



std::pair<Collision_Group, Collision_Group> Collision::Get_Pair() {
	//Always keep the lesser group id in front
	if (lhs->group < rhs->group)
		return std::make_pair(lhs->group, rhs->group);
	else
		return std::make_pair(rhs->group, lhs->group);
}