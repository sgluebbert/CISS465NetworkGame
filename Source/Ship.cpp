#include "Ship.h"



void Ship::Set_Texture(Texture * tex, float width, float height) {
    texture = tex;
    w = width;
    h = height;
}

void Ship::Fire(int weapon_id) {
	//weapon_pool[weapond_id]->Fire();
}

void Ship::Update(double dt) {
	Entity::Update(dt);

	smoke_emitter.Update(dt);
	explosion_emitter.Update(dt);

	switch(state) {
	case ALIVE:
	    if (health <= 0.5 * max_health)
	        smoke_emitter.Activate();

	    if (health <= 0.0) {
	        explosion_emitter.Activate();
	        state = DYING;
	    }

		break;

	case DYING:
		if (explosion_emitter.Is_Active())
			state = DEAD;

		break;

	case DEAD:
		smoke_emitter.Deactivate();
		explosion_emitter.Deactivate();

		break;
	}
}

void Ship::Draw() {
    if (texture == NULL)
        return;
    if (state == DEAD || state == DYING)
        return;

    texture->DrawCentered(x, y, -angle, 48);
}