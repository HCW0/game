#include "kinematic.h"
#include "particle.h"




	void Kinematic::basic_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle &pt, float dt) {
		// �ܼ��� ���ӵ� ��Ģ�̹Ƿ�, �ӵ��� ���ӵ���ŭ ���ϰ�, ��ġ�� �ӵ���ŭ ���Ѵ�.
		v->setY(v->getY() + pt.gravity);
		*v = *v + *a;
		*v = *v*0.9999;
		if (v->getScale() < 1.) v->setVector(.0, .0, .0);
		*p = *p + *v*pt.cnt_dt*pt.cnt_dt;
		//cout << "trsplg : " << pt.transpology << endl;
	}

	void Kinematic::circular_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle &pt, float dt) {
		// ��ӷ� ����̸�, ��ġ ���� ������ [cos t , sin t , 0]���� ����ϸ� ���ϳ�
		// �ӵ��� ���ӵ��� ����� �� ��� �浹�̳� ���� �ݻ�� ���� ������ ó���� �� ������
		// ���ŷ����� ���ӵ��� �ӵ��� ���� ���ؼ� ���ӵ��� ��Ģ�� ���� ��ġ�� �����ؾ߸� �Ѵ�.
		// 

		double radius = 1.*1./pt.cnt_dt;
		double angular_velocity = 1.;
		double radian = dt*M_PI / 180.;
		v->setVector(-1 * radius*angular_velocity*sin(angular_velocity*radian), radius*angular_velocity*cos(angular_velocity*radian), 0);
		// v = [-rw sin(w*dt) , rw cos(w*dt) , 0]
		a->setVector(-1 * radius*angular_velocity*angular_velocity*cos(angular_velocity*radian), -1 * radius*angular_velocity*angular_velocity*sin(angular_velocity*radian), 0);
		// a = [-rww cos, -rww sin, 0] 
		basic_kinematic(p, v, a, pt, dt);
		// �ӵ�, ���ӵ��� �������� ���ӵ� ��Ģ���� �ѱ�
		
	}

	void Kinematic::pendulum_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle &pt, float dt) {
		// ��ⷳ ��̰�, ���� ���� ���ǻ� �������� �д�.
		// ���ʿ� ��ü�� ������ ��ġ = ���콺 Ŭ���� ��ġ�� ������ �̷�� ������ 
		// ������ �ִ� � ���� ��0�� �ȴ�.
		//
		// �ش� ���� ��� circular correction �� end point correction�� ����Ǿ� ���� �ʴ�.
		// �׷��� ���� �˵��� ��� ���� ��ü�� �����ϸ�, ��ü�� �˵��� ��Ż�Ͽ� ���� �����ο� ��� �ϴµ�
		// �� ��� ���� ��� �����̶� ���ܵξ���.
		//
		double coord_x = .0, coord_y = .0, coord_z = .0, string_length = .0;
		// ������ � �� �� ������ �����̴�. ���� ���� �����̸� ���� Ȯ���� ���� ������ �� �ְ� �����Ѵ�.
		// ������ ���̴� �ʱ����� x0,y0,z0�� ������ ���̷� ��´�.
		//
		MyVector g(.0, -.98, .0);
		// �߷� ����
		//
		MyVector in_vector0(coord_x - pt.x0, coord_y - pt.y0, coord_z - pt.z0);
		// ���� �ʱ� ��ġ���� ���� �߽����� ���ϴ� ���͸� IN0 ���ͷ� ��´�.
		string_length = in_vector0.getScale();
		// ������ ���̴� IN0 ������ ũ��� ����.
		//
		MyVector in_vector(coord_x - p->getX(), coord_y - p->getY(), coord_z - p->getZ());
		// ���� ��ġ���� ���� �߽����� ���ϴ� IN ����
		//
		double cossine = in_vector*(g*-1.) / (g.getScale()*in_vector.getScale());
		double cossine0 = in_vector0*(g*-1.) / (g.getScale()*in_vector0.getScale());
		//
		//
		// ���� ���� ������
		MyVector whole_accel = g + in_vector*(3.*cossine - 2.*cossine0)*(g.getScale() / string_length);
		// ���ڰ��ӵ� = g + (3cos�� - 2cos��0) * IN * |g|/|IN|
		*a = whole_accel;
		*v = *v + *a;
		// �ӵ�, ���ӵ��� �������� ���ӵ� ��Ģ���� �ѱ�

		if (cossine - cossine0 < 0.) {
			*v = *v*-1.;
		}
		// end point correction
		// ���� ���ڰ� ���� �����ع����ٸ�, �˵��� �Ѿ������ �ʰ� ���ӵ��� �ݴ�� �����ش�.

		basic_kinematic(p, v, a, pt, dt);
		// ���ӵ� ��Ģ�� ����, �ӵ� �� ��ġ �� ����

		*p = *p*(string_length / p->getScale());
		// circular correction

	}



	void Kinematic::pseudo_pendulum_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt) {
		// ��ⷳ ��̰�, ���� ���� ���ǻ� �������� �д�.
		// ���ʿ� ��ü�� ������ ��ġ = ���콺 Ŭ���� ��ġ�� ������ �̷�� ������ 
		// ������ �ִ� � ���� ��0�� �ȴ�.
		//
		// �ش� ���� ��� circular correction �� end point correction�� ����Ǿ� ���� �ʴ�.
		// �׷��� ���� �˵��� ��� ���� ��ü�� �����ϸ�, ��ü�� �˵��� ��Ż�Ͽ� ���� �����ο� ��� �ϴµ�
		// �� ��� ���� ��� �����̶� ���ܵξ���.
		//
		double coord_x = .0, coord_y = .0, coord_z = .0, string_length = .0;
		// ������ � �� �� ������ �����̴�. ���� ���� �����̸� ���� Ȯ���� ���� ������ �� �ְ� �����Ѵ�.
		// ������ ���̴� �ʱ����� x0,y0,z0�� ������ ���̷� ��´�.
		//
		MyVector g(.0, -.98, .0);
		// �߷� ����
		//
		MyVector in_vector0(coord_x - pt.x0, coord_y - pt.y0, coord_z - pt.z0);
		// ���� �ʱ� ��ġ���� ���� �߽����� ���ϴ� ���͸� IN0 ���ͷ� ��´�.
		string_length = in_vector0.getScale();
		// ������ ���̴� IN0 ������ ũ��� ����.
		//
		MyVector in_vector(coord_x - p->getX(),coord_y - p->getY(), coord_z - p->getZ());
		// ���� ��ġ���� ���� �߽����� ���ϴ� IN ����
		//
		double cossine = in_vector*(g*-1.) / (g.getScale()*in_vector.getScale());
		double cossine0 = in_vector0*(g*-1.) / (g.getScale()*in_vector0.getScale());
		//
		//
		// ���� ���� ������
		MyVector whole_accel = g + in_vector*(3.*cossine - 2.*cossine0)*(g.getScale() / string_length);
		// ���ڰ��ӵ� = g + (3cos�� - 2cos��0) * IN * |g|/|IN|

		*a = whole_accel;
		*v = *v + *a;
		basic_kinematic(p, v, a, pt, dt);
		// �ӵ�, ���ӵ��� �������� ���ӵ� ��Ģ���� �ѱ�
	}



	void Kinematic::cycloid_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt) {
		// ������ p(t) = p0 + radius*[dt - sin dt , 1 - cos dt , 0 ] �� ��ġ�� ����Ͽ�
		// �浹�� ����� �ӵ��� ���ӵ��� ���� �� ���� ������, ��ġ ���� �������� �̺��Ͽ� �ӵ�/���ӵ���
		// ���ϰ� ���ӵ��� ��Ģ�� �����ϴ� ������ ����Ŭ���̵� ��� ����Ͽ���.
		//
		double radius = .1*1./pt.cnt_dt;
		double radian = dt*M_PI / 180.;
		v->setVector(radius*(radius + 1.) - radius *cos(radian), radius*sin(radian), 0);
		// radius+1. �� �����ִ� ������ curtate cycloid�� �����ϱ� ����, normal cycloid�� �����
		// �; ������ ���ڸ� ã�� ���߿� �ش� ����ġ�� ���� �����ϰ� ���� �������� �����̴�.
		a->setVector(radius*sin(radian), radius*cos(radian), 0);
		basic_kinematic(p, v, a, pt, dt);
	}

	void Kinematic::stair_up_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt) {
		// dt�� ���������� ĳ�����ؼ�, �Ҽ������� ������ ����Ŭ���̵� ��ġ ���� ������ p(t)�� �ӵ����ͷ� ��� ��̴�.
		// ��ġ ����� ������ �� ���� ��� �Ѵ�.
		//
		//
		int timetheta = (int)dt % 360;
		double radius = .6;
		double radian = timetheta*M_PI / 180.;
		v->setVector(radian - sin(radian), 1. - cos(radian), 0);
		*v = *v*radius*.1;
		basic_kinematic(p, v, a, pt, dt);
	}

	void Kinematic::_2D_helix_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt) {
		// ������ ���ϸ�, curtate cycloid�� �����̳� Trace mode�� �Ѱ� �����ϸ�
		// ��ġ 3���� ������ Helix ������ ���� �� ���Ƽ� ���ܵξ���.
		//
		//
		int timetheta = (int)dt;
		double radius = .03;
		double radian = (timetheta)*M_PI / 180.;
		v->setVector(radius - radius *cos(radian), sin(radian), 0);
		a->setVector(sin(radian), cos(radian), 0);
		basic_kinematic(p, v, a, pt, dt);
	}

	void Kinematic::one_one_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt) {
		// Ű���� ȭ��ǥŰ�� ���� ������ �� �ִ� ��ü�� ���� ��̴�.
		//
		//
		p->setVector(pt.control_X, pt.control_Y, 0);
		basic_kinematic(p, v, a, pt, dt);
	}




