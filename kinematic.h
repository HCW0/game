#ifndef _KINEMA4_
#define _KINEMA4_

#include "MyVector.cpp"
class Particle;
class Kinematic {
private:

public:
	void basic_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle &pt, float dt);

	void circular_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle &pt, float dt);

	void pendulum_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt);

	void pseudo_pendulum_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt);

	void cycloid_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt);

	void stair_up_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt);

	void _2D_helix_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt);

	void one_one_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt);

};
#endif



