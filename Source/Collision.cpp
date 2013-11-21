#include "Collision.h"



/*Ship Collision Callbacks*/
/////////////////////////////////////////////////////////
void ShipToShipCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * lhs_ship = (Ship *) lhs;
	Ship * rhs_ship = (Ship *) rhs;
}

void ShipToBulletCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	//Bullet * bullet = (Bullet *) rhs;
}

void ShipToPlanetCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	Planet * planet = (Planet *) rhs;

    if (planet->locked)
        return;

    if (ship->team_id == RED_TEAM)
        planet->alignment -= planet->capture_rate * ship->capture_modifier;
    else
        planet->alignment += planet->capture_rate * ship->capture_modifier;


    if (planet->alignment > 1.0f) {
        planet->alignment = 1.0f;
        planet->team_id = BLUE_TEAM;
        planet->texture = surface_manager->blue_planet;

        /*Lock the right neighbor*/
        //////////////////////////////////////////////////
        Planet * neighbor = NULL;

        for (std::list<Planet *>::iterator it = Planet::planet_graph.begin(); it != Planet::planet_graph.end(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.end())
                    neighbor = *(++it);

        if (neighbor != NULL)
            neighbor->locked = true;
        //////////////////////////////////////////////////

        /*Unlock the left neighbor*/
        //////////////////////////////////////////////////
        neighbor = NULL;

        for (std::list<Planet *>::reverse_iterator it = Planet::planet_graph.rbegin(); it != Planet::planet_graph.rend(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.rend())
                    neighbor = *(++it);

        if (neighbor != NULL)
            neighbor->locked = false;
        //////////////////////////////////////////////////
    }

    if (planet->alignment < -1.0f) {
        planet->alignment = -1.0f;
        planet->team_id = RED_TEAM;
        planet->texture = surface_manager->red_planet;

        /*Lock the left neighbor*/
        //////////////////////////////////////////////////
        Planet * neighbor = NULL;

        for (std::list<Planet *>::reverse_iterator it = Planet::planet_graph.rbegin(); it != Planet::planet_graph.rend(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.rend())
                    neighbor = *(++it);

        if (neighbor != NULL)
            neighbor->locked = true;
        //////////////////////////////////////////////////

        /*Unlock the right neighbor*/
        //////////////////////////////////////////////////
        neighbor = NULL;

        for (std::list<Planet *>::iterator it = Planet::planet_graph.begin(); it != Planet::planet_graph.end(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.end())
                    neighbor = *(it);

        if (neighbor != NULL)
            neighbor->locked = false;
        //////////////////////////////////////////////////
    }

    if (planet->team_id == BLUE_TEAM && planet->alignment < 0.0f) {
        planet->team_id = NEUTRAL_TEAM;

        /*Lock the left neighbor*/
        //////////////////////////////////////////////////
        Planet * neighbor = NULL;

        for (std::list<Planet *>::reverse_iterator it = Planet::planet_graph.rbegin(); it != Planet::planet_graph.rend(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.rend())
                    neighbor = *(++it);

        if (neighbor != NULL) {
            neighbor->locked = true;
            neighbor->alignment = -1.0;
        }
        //////////////////////////////////////////////////
    }

    if (planet->team_id == RED_TEAM && planet->alignment > 0.0f) {
        planet->team_id = NEUTRAL_TEAM;

        /*Lock and reset the right neighbor*/
        //////////////////////////////////////////////////
        Planet * neighbor = NULL;

        for (std::list<Planet *>::iterator it = Planet::planet_graph.begin(); it != Planet::planet_graph.end(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.end())
                    neighbor = *(++it);

        if (neighbor != NULL) {
            neighbor->locked = true;
            neighbor->alignment = 1.0;
        }
        //////////////////////////////////////////////////
    }

    if (planet->alignment == 0.0f || planet->team_id == NEUTRAL_TEAM)
        planet->texture = surface_manager->neutral_planet;
}

void ShipToMoonCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	//Moon * moon = (Moon *) rhs;
}

void ShipToAsteroidCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	//Asteroid * asteroid = (Asteroid *) rhs;
}

void ShipToFactoryCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	//Factory * factory = (Factory *) rhs;
}
/////////////////////////////////////////////////////////


/*Bullet Collision Callbacks*/
/////////////////////////////////////////////////////////
void BulletToPlanetCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	//Bullet * bullet = (Bullet *) lhs;
	Planet * planet = (Planet *) rhs;
}

void BulletToMoonCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	//Bullet * bullet = (Bullet *) lhs;
	//Moon * moon = (Moon *) rhs;
}

void BulletToAsteroidCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	//Bullet * bullet = (Bullet *) lhs;
	//Asteroid * asteroid = (Asteroid *) rhs;
}
/////////////////////////////////////////////////////////


/*Planet Collision Callbacks*/
/////////////////////////////////////////////////////////
void PlanetToAsteroidCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Planet * planet = (Planet *) lhs;
	//Asteroid * asteroid = (Asteroid *) rhs;
}
/////////////////////////////////////////////////////////


/*Moon Collision Callbacks*/
/////////////////////////////////////////////////////////
void MoonToAsteroidCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	//Moon * moon = (Moon *) lhs;
	//Asteroid * asteroid = (Asteroid *) rhs;
}
/////////////////////////////////////////////////////////