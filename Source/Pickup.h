#ifndef PICKUP_H
#define PICKUP_H



#include "Ship.h"



class Pickup {
public:
	virtual void AffectShip(Ship *) = 0;
	virtual bool Expired();
};



class Collectible : public Pickup {
public:
	virtual void AffectShip(Ship *);
	virtual bool Expired();
};



/*class Powerup : public Pickup {
public:
	virtual void AffectShip(Ship *);
	virtual bool Expired();

	Timer age_timer;
};*/



#endif