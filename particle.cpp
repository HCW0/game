#include "particle.h"

Particle::Particle(double mass, double radius) : mass(mass), radius(radius) {
	p = new MyVector(.0, .0, .0);
	v = new MyVector(.0, .0, .0);
	a = new MyVector(.0, .0, .0);
	k = new Kinematic();
}

void Particle::apply_gravity() {
	if (gravity_flag) {
		gravity = -0.098;
	}
	else {
		gravity = 0;
	}
}

void Particle::run(double cnt_dt) {
	MyVector p_prev;
	this->cnt_dt = cnt_dt;
		apply_gravity();
		p_prev = *p;
		cnt += 1. * cnt_dt;

			switch (k_type) {
			case 1:
				k->circular_kinematic(p, v, a, *this, cnt);
				break;
			case 2:
				k->pendulum_kinematic(p, v, a, *this, cnt);
				break;
			case 3:
				k->cycloid_kinematic(p, v, a, *this, cnt);
				break;
			case 4:
				k->one_one_kinematic(p, v, a, *this, cnt);
				break;
			default :
				k->basic_kinematic(p, v, a, *this, cnt);
				break;
			}
		
		transpology = p->getEuclidean(p_prev) - 2.*radius;
}




MyVector*& Particle::getPosition() { return this->p; }
MyVector*& Particle::getVelocity() { return this->v; }
MyVector*& Particle::getAcceleration() { return this->a; }

void Particle::setStopFlag(bool flag) {
	isStop = flag;
}
bool Particle::getStopFlag() {
	return isStop;
}
void Particle::setPosition(double x, double y, double z) {
	this->p->setX(x);
	this->p->setY(y);
	this->p->setZ(z);
}
void Particle::save_first_position(double x, double y, double z) {
	this->x0 = x;
	this->y0 = y;
	this->z0 = z;
}
void Particle::setAcceleration(double x, double y, double z) {
	this->a->setX(x);
	this->a->setY(y);
	this->a->setZ(z);
}
void Particle::setVelocity(double x, double y, double z) {
	this->v->setX(x);
	this->v->setY(y);
	this->v->setZ(z);
}
void Particle::setControl(double x, double y) {
	this->control_X = x;
	this->control_Y = y;
}
void Particle::setKinematic(int k) {
	this->k_type = k;
}
int Particle::getKinematic() {
	return this->k_type;
}
double Particle::getRadius() { return this->radius; }




