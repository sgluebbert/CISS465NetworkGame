#ifndef SHIP_H
#define SHIP_H



#include "Collidable.h"
#include "Entity.h"
#include "Motor.h"
#include "Particles/ParticleManager.h"
#include "Serializable.h"
#include "Weapon.h"



enum Ship_State	 {	ALIVE, DYING, DEAD	};
enum Ship_Type	 {	INTERCEPTOR, FIGHTER, FRIGATE, BOMBER	};
enum Weapon_Type {	ENERGY_TYPE, BALLISTIC_TYPE, PROPELLED_TYPE, BOMB_TYPE, POWERUP_TYPE	};


struct SpawnPoint
{
	SpawnPoint()
		: x(0), y(0)
	{}
	SpawnPoint(float _x, float _y)
		: x(_x), y(_y)
	{}

	float x, y;
};



class Ship : public Entity, public Collidable, public Motor, public Serializable {
public:
	Ship(Ship_Type, float, float);
	~Ship();
	
    NetString * Serialize();
    bool Deserialize(NetString *);

	bool Fire(Weapon_Type, bool forced = false); // if client got a ship shot from the server, we force the shot to stay in sync
	void Accelerate(bool);
	void Decelerate();
    void Turn_Left(double);
    void Turn_Right(double);

	void Damage_Armor(float);
	void Damage_Shields(float);
	void Damage_Hull(float);
    
    void Limit_Motor();
	void Update(double);
	void Draw();
	void Sync();

	Texture * texture_c;

    Particle_Emitter smoke_emitter;
    Particle_Emitter shatter_emitter;

    Ship_State state;
    std::string player_name;

    Timer respawn_timer;
	Timer power_recharge_timer;
	Timer shield_recharge_timer;

	float max_health, max_shields, max_armor, max_power;
	float health, shields, armor, power;
	float shield_recharge_rate;
	float power_recharge_rate;
	float capture_modifier;
    float turn_rate;
    int captures;
    int kills;
    int shots_hit;
	Weapon * weapon_pool[5];

	static const float default_max_resource, default_recharge_delay, default_recharge_rate, default_capture_modifier;
	static std::deque<Ship *> ships;

	static int Add_Ship(Team, Ship_Type, float, float, float);
	static void Remove_Ship(int);
	static void Remove_Ship(Ship *);

	static void Initialize_Ships(int);
	static void Cleanup_Ships();

private:

	void Setup_Interceptor();
	void Setup_Fighter();
	void Setup_Frigate();
	void Setup_Bomber();

	static int ship_count;
};



#endif