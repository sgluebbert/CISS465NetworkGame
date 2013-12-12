#include "Collision.h"
#include "Clock.h"



/*Ship Collision Callbacks*/
/////////////////////////////////////////////////////////
void ShipToShipCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * lhs_ship = (Ship *) lhs;
	Ship * rhs_ship = (Ship *) rhs;
}

void ShipToBulletCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	Particle * bullet = (Particle *) rhs;
}

void ShipToPlanetCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	Planet * planet = (Planet *) rhs;
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

    if (planet->locked)
        return;

    if (ship->team_id == RED_TEAM && planet->alignment > -1.0)
        planet->alignment -= planet->capture_rate * ship->capture_modifier * dt;
    else if (ship->team_id == BLUE_TEAM && planet->alignment < 1.0)
        planet->alignment += planet->capture_rate * ship->capture_modifier * dt;


    if (planet->alignment > 1.0f) {
        planet->alignment = 1.0f;
        if (planet->team_id != BLUE_TEAM)
        {
            planet->just_captured = true;
            ship->captures++;
        }
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
            neighbor->Lock(true);
        //////////////////////////////////////////////////

        /*Unlock the left neighbor*/
        //////////////////////////////////////////////////
        neighbor = NULL;

        for (std::list<Planet *>::reverse_iterator it = Planet::planet_graph.rbegin(); it != Planet::planet_graph.rend(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.rend())
                    neighbor = *(++it);

        if (neighbor != NULL)
            neighbor->Lock(false);
        //////////////////////////////////////////////////
    }

    else if (planet->alignment < -1.0f) {
        planet->alignment = -1.0f;
        if (planet->team_id != RED_TEAM)
        {
            planet->just_captured = true;
            ship->captures++;
        }
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
            neighbor->Lock(true);
        //////////////////////////////////////////////////

        /*Unlock the right neighbor*/
        //////////////////////////////////////////////////
        neighbor = NULL;

        for (std::list<Planet *>::iterator it = Planet::planet_graph.begin(); it != Planet::planet_graph.end(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.end())
                    neighbor = *(++it);

        if (neighbor != NULL)
            neighbor->Lock(false);
        //////////////////////////////////////////////////
    }

    else if (planet->team_id == BLUE_TEAM && planet->alignment < 0.0f) {
        planet->team_id = NEUTRAL_TEAM;
        planet->texture = surface_manager->neutral_planet;

        /*Lock the left neighbor*/
        //////////////////////////////////////////////////
        Planet * neighbor = NULL;

        for (std::list<Planet *>::reverse_iterator it = Planet::planet_graph.rbegin(); it != Planet::planet_graph.rend(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.rend())
                    neighbor = *(++it);

        if (neighbor != NULL)
            neighbor->Lock(true);
        //////////////////////////////////////////////////
    }

    else if (planet->team_id == RED_TEAM && planet->alignment > 0.0f) {
        planet->team_id = NEUTRAL_TEAM;
        planet->texture = surface_manager->neutral_planet;

        /*Lock and reset the right neighbor*/
        //////////////////////////////////////////////////
        Planet * neighbor = NULL;

        for (std::list<Planet *>::iterator it = Planet::planet_graph.begin(); it != Planet::planet_graph.end(); ++it)
            if (*it == planet)
                if ((++it)-- != Planet::planet_graph.end())
                    neighbor = *(++it);

        if (neighbor != NULL)
            neighbor->Lock(true);
        //////////////////////////////////////////////////
    }
}

void ShipToMoonCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	Moon * moon = (Moon *) rhs;
    double dt = Clock::Frame_Control.Get_Time_Per_Frame();

    if (ship->team_id == RED_TEAM && moon->alignment > -1.0)
        moon->alignment -= moon->capture_rate * ship->capture_modifier * dt;
    else if (ship->team_id == BLUE_TEAM && moon->alignment < 1.0)
        moon->alignment += moon->capture_rate * ship->capture_modifier * dt;


    if (moon->alignment > 1.0) {
        moon->alignment = 1.0;
        if (moon->team_id != BLUE_TEAM)
            moon->just_captured = true;
        moon->team_id = BLUE_TEAM;

        moon->DistributeResource();
    }

    else if (moon->alignment < -1.0) {
        moon->alignment = -1.0;
        if (moon->team_id != RED_TEAM)
            moon->just_captured = true;
        moon->team_id = RED_TEAM;

        moon->DistributeResource();
    }

    else if (moon->team_id == BLUE_TEAM && moon->alignment < 0.0) {
        moon->team_id = NEUTRAL_TEAM;
        moon->RemoveResource(BLUE_TEAM);
    }

    else if (moon->team_id == RED_TEAM && moon->alignment > 0.0) {
        moon->team_id = NEUTRAL_TEAM;
        moon->RemoveResource(RED_TEAM);
    }
}

void ShipToAsteroidCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	//Asteroid * asteroid = (Asteroid *) rhs;
}

void ShipToFactoryCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
	Ship * ship = (Ship *) lhs;
	Pickup_Factory * factory = (Pickup_Factory *) rhs;

    Pickup * pickup = factory->Grab_Pickup();
    pickup->OnPickup(ship);
}
/////////////////////////////////////////////////////////


/*Bullet Collision Callbacks*/
/////////////////////////////////////////////////////////
void BulletToPlanetCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
    Particle * bullet = (Particle *) lhs;
	Planet * planet = (Planet *) rhs;

    planet->bounding_volume.r -= planet->gravity_radius;

    if (DoCollide(planet, bullet))
        bullet->distance_travelled = 9999.9999;

    planet->bounding_volume.r += planet->gravity_radius;
}

void BulletToMoonCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
    Particle * bullet = (Particle *) lhs;
	Moon * moon = (Moon *) rhs;

    moon->bounding_volume.r -= moon->field_radius;

    if (DoCollide(moon, bullet)) {
        moon->TakeDamage(50);
        bullet->distance_travelled = 9999.9999;
    }

    moon->bounding_volume.r += moon->field_radius;

}

void BulletToAsteroidCollision::ResolveCollision(Collidable * lhs, Collidable * rhs) {
    Particle * bullet = (Particle *) lhs;
	//Asteroid * asteroid = (Asteroid *) rhs;

    bullet->distance_travelled = 9999.9999;
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