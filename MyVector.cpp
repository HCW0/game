#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

class MyVector {

private:
	double x_dir, y_dir, z_dir;
public:
	MyVector(double x_dir=.0, double y_dir=.0, double z_dir=.0) :x_dir(x_dir), y_dir(y_dir), z_dir(z_dir) {}


	long double getScale() {
		return sqrtl(powl(x_dir, 2.) + powl(y_dir, 2.) + powl(z_dir, 2.));
	}

	double getX() const {
		return this->x_dir;
	}

	void setX(double x) {
		this->x_dir = x;
	}

	double getY() const {
		return this->y_dir;
	}

	void setY(double y) {
		this->y_dir = y;
	}

	double getZ() const {
		return this->z_dir;
	}

	void setZ(double z) {
		this->z_dir = z;
	}

	void setZero() {
		this->x_dir = this->y_dir = this->z_dir = 0;
	}

	void setVector(MyVector*& c) {
		this->x_dir = c->getX();
		this->y_dir = c->getY();
		this->z_dir = c->getZ();
	}

	void setVector(MyVector const & c) {
		this->x_dir = c.getX();
		this->y_dir = c.getY();
		this->z_dir = c.getZ();
	}

	void setVector(double x_dir, double y_dir, double z_dir) {
		this->x_dir = x_dir;
		this->y_dir = y_dir;
		this->z_dir = z_dir;
	}

	double getEuclidean(const MyVector &m) {
		double result = powl(m.getX() - this->getX(), 2) + powl(m.getY() - this->getY(), 2) + powl(m.getZ() - this->getZ(), 2);
		return sqrtl(result);
	}

	// operator overloading

	const MyVector operator +(const MyVector &m) const {
		double x = this->getX() + m.getX();
		double y = this->getY() + m.getY();
		double z = this->getZ() + m.getZ();
		MyVector result(x, y, z);
		return result;
	}

	const double operator *(const MyVector &m) const {
		double x = this->getX() * m.getX();
		double y = this->getY() * m.getY();
		double z = this->getZ() * m.getZ();
		return x+y+z;
	}// inner product

	const MyVector operator *(const double s) const {
		double x = this->getX() * s;
		double y = this->getY() * s;
		double z = this->getZ() * s;
		MyVector result(x, y, z);
		return result;
	}

	MyVector& operator =(const MyVector &m) {
		this->setX(m.getX());
		this->setY(m.getY());
		this->setZ(m.getZ());
		return *this;
	}

};


