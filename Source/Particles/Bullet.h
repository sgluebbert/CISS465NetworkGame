#ifndef BULLET_H
#define BULLET_H



class Bullet : public Particle, public Collidable {
public:
	Bullet();
	Bullet(Particle *, float &, float &);
	~Bullet();
    
    virtual void Limit_Motor();
    virtual void Update(double);
    virtual void Draw();

    float distance_travelled, range;
};



#endif