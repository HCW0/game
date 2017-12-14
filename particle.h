
#ifndef _PARTICLE4_
#define _PARTICLE4_
#include <iostream>
#include "kinematic.h"

class Kinematic;

class Particle {

private:
	double radius;
	bool isCollide = true, isStop = false, isGravity = false;
	MyVector *p, *v, *a;
	Kinematic *k;
	int k_type;


public:
	int id;
	int c_type;
	double mass;
	double transpology;
	bool gravity_flag;
	double gravity,cnt_dt;
	double control_X, control_Y;
	double x0, y0, z0;
	double cnt, gradation; // 시간의 경과를 나타내는 변수.
	Particle() {}
	Particle(double mass, double radius);

	void run(double cnt_dt);
	MyVector*& getPosition();
	MyVector*& getVelocity();
	MyVector*& getAcceleration();

	void apply_gravity();
	void setStopFlag(bool flag);
	bool getStopFlag();
	void setPosition(double x, double y, double z);
	void save_first_position(double x, double y, double z);
	void setAcceleration(double x, double y, double z);
	void setVelocity(double x, double y, double z);
	void setControl(double x, double y);
	void setKinematic(int k);
	int getKinematic();
	double getRadius();

};
#endif