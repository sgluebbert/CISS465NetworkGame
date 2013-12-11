#ifndef PICKUP_H
#define PICKUP_H



#include "Ship.h"



class Pickup {
public:
	Texture * texture;
	
	virtual void OnPickup(Ship *) = 0;
	virtual void Update(Ship *) = 0;
	virtual bool OnExpiration(Ship *) = 0;
};



enum Collectible_Type {	HEALTH_COLLECTIBLE, SHIELDS_COLLECTIBLE, ARMOR_COLLECTIBLE, POWER_COLLECTIBLE	};

class Collectible : public Pickup {
public:
	Collectible();

	//Pickup_State state;
	Collectible_Type type;

	virtual void OnPickup(Ship *);
	virtual void Update(Ship *);
	virtual bool OnExpiration(Ship *);
};



/*class Powerup : public Pickup {
public:
	virtual void AffectShip(Ship *);
	virtual bool Expired();

	Timer age_timer;
};*/



#endif