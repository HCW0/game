#include "kinematic.h"
#include "particle.h"




	void Kinematic::basic_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle &pt, float dt) {
		// 단순히 가속도 법칙이므로, 속도를 가속도만큼 더하고, 위치를 속도만큼 더한다.
		v->setY(v->getY() + pt.gravity);
		*v = *v + *a;
		*v = *v*0.9999;
		if (v->getScale() < 1.) v->setVector(.0, .0, .0);
		*p = *p + *v*pt.cnt_dt*pt.cnt_dt;
		//cout << "trsplg : " << pt.transpology << endl;
	}

	void Kinematic::circular_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle &pt, float dt) {
		// 등속력 원운동이며, 위치 벡터 방정식 [cos t , sin t , 0]으로 기술하면 편리하나
		// 속도와 가속도를 기술할 수 없어서 충돌이나 경계면 반사와 같은 로직을 처리할 수 없으니
		// 번거럽더라도 가속도와 속도를 각각 구해서 가속도의 법칙에 의한 위치를 산출해야만 한다.
		// 

		double radius = 1.*1./pt.cnt_dt;
		double angular_velocity = 1.;
		double radian = dt*M_PI / 180.;
		v->setVector(-1 * radius*angular_velocity*sin(angular_velocity*radian), radius*angular_velocity*cos(angular_velocity*radian), 0);
		// v = [-rw sin(w*dt) , rw cos(w*dt) , 0]
		a->setVector(-1 * radius*angular_velocity*angular_velocity*cos(angular_velocity*radian), -1 * radius*angular_velocity*angular_velocity*sin(angular_velocity*radian), 0);
		// a = [-rww cos, -rww sin, 0] 
		basic_kinematic(p, v, a, pt, dt);
		// 속도, 가속도가 정해지면 가속도 법칙으로 넘김
		
	}

	void Kinematic::pendulum_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle &pt, float dt) {
		// 펜듈럼 운동이고, 진자 축은 편의상 원점으로 둔다.
		// 최초에 강체를 생성한 위치 = 마우스 클릭한 위치와 원점이 이루는 각도가 
		// 진자의 최대 운동 각도 Θ0가 된다.
		//
		// 해당 진자 운동은 circular correction 및 end point correction이 적용되어 있지 않다.
		// 그래서 일정 궤도를 벗어난 곳에 강체를 생성하면, 강체가 궤도를 이탈하여 아주 자유로운 운동을 하는데
		// 그 모습 또한 운동의 일종이라 남겨두었다.
		//
		double coord_x = .0, coord_y = .0, coord_z = .0, string_length = .0;
		// 진자의 운동 축 및 연선의 길이이다. 진자 축은 원점이며 추후 확장을 통해 변경할 수 있게 구현한다.
		// 연선의 길이는 초기지점 x0,y0,z0와 원점의 길이로 잡는다.
		//
		MyVector g(.0, -.98, .0);
		// 중력 벡터
		//
		MyVector in_vector0(coord_x - pt.x0, coord_y - pt.y0, coord_z - pt.z0);
		// 진자 초기 위치에서 진자 중심으로 향하는 벡터를 IN0 벡터로 잡는다.
		string_length = in_vector0.getScale();
		// 연직선 길이는 IN0 벡터의 크기와 같다.
		//
		MyVector in_vector(coord_x - p->getX(), coord_y - p->getY(), coord_z - p->getZ());
		// 현재 위치에서 진자 중심으로 향하는 IN 벡터
		//
		double cossine = in_vector*(g*-1.) / (g.getScale()*in_vector.getScale());
		double cossine0 = in_vector0*(g*-1.) / (g.getScale()*in_vector0.getScale());
		//
		//
		// 진자 벡터 방정식
		MyVector whole_accel = g + in_vector*(3.*cossine - 2.*cossine0)*(g.getScale() / string_length);
		// 진자가속도 = g + (3cosΘ - 2cosΘ0) * IN * |g|/|IN|
		*a = whole_accel;
		*v = *v + *a;
		// 속도, 가속도가 정해지면 가속도 법칙으로 넘김

		if (cossine - cossine0 < 0.) {
			*v = *v*-1.;
		}
		// end point correction
		// 만약 진자가 끝에 도달해버린다면, 궤도를 넘어가버리지 않게 가속도를 반대로 고쳐준다.

		basic_kinematic(p, v, a, pt, dt);
		// 가속도 법칙에 의한, 속도 및 위치 값 갱신

		*p = *p*(string_length / p->getScale());
		// circular correction

	}



	void Kinematic::pseudo_pendulum_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt) {
		// 펜듈럼 운동이고, 진자 축은 편의상 원점으로 둔다.
		// 최초에 강체를 생성한 위치 = 마우스 클릭한 위치와 원점이 이루는 각도가 
		// 진자의 최대 운동 각도 Θ0가 된다.
		//
		// 해당 진자 운동은 circular correction 및 end point correction이 적용되어 있지 않다.
		// 그래서 일정 궤도를 벗어난 곳에 강체를 생성하면, 강체가 궤도를 이탈하여 아주 자유로운 운동을 하는데
		// 그 모습 또한 운동의 일종이라 남겨두었다.
		//
		double coord_x = .0, coord_y = .0, coord_z = .0, string_length = .0;
		// 진자의 운동 축 및 연선의 길이이다. 진자 축은 원점이며 추후 확장을 통해 변경할 수 있게 구현한다.
		// 연선의 길이는 초기지점 x0,y0,z0와 원점의 길이로 잡는다.
		//
		MyVector g(.0, -.98, .0);
		// 중력 벡터
		//
		MyVector in_vector0(coord_x - pt.x0, coord_y - pt.y0, coord_z - pt.z0);
		// 진자 초기 위치에서 진자 중심으로 향하는 벡터를 IN0 벡터로 잡는다.
		string_length = in_vector0.getScale();
		// 연직선 길이는 IN0 벡터의 크기와 같다.
		//
		MyVector in_vector(coord_x - p->getX(),coord_y - p->getY(), coord_z - p->getZ());
		// 현재 위치에서 진자 중심으로 향하는 IN 벡터
		//
		double cossine = in_vector*(g*-1.) / (g.getScale()*in_vector.getScale());
		double cossine0 = in_vector0*(g*-1.) / (g.getScale()*in_vector0.getScale());
		//
		//
		// 진자 벡터 방정식
		MyVector whole_accel = g + in_vector*(3.*cossine - 2.*cossine0)*(g.getScale() / string_length);
		// 진자가속도 = g + (3cosΘ - 2cosΘ0) * IN * |g|/|IN|

		*a = whole_accel;
		*v = *v + *a;
		basic_kinematic(p, v, a, pt, dt);
		// 속도, 가속도가 정해지면 가속도 법칙으로 넘김
	}



	void Kinematic::cycloid_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt) {
		// 기존의 p(t) = p0 + radius*[dt - sin dt , 1 - cos dt , 0 ] 는 위치만 기술하여
		// 충돌에 사용할 속도나 가속도를 구할 수 없기 때문에, 위치 벡터 방정식을 미분하여 속도/가속도를
		// 구하고 가속도의 법칙을 적용하는 것으로 싸이클로이드 운동을 기술하였다.
		//
		double radius = .1*1./pt.cnt_dt;
		double radian = dt*M_PI / 180.;
		v->setVector(radius*(radius + 1.) - radius *cos(radian), radius*sin(radian), 0);
		// radius+1. 을 곱해주는 이유는 curtate cycloid를 방지하기 위해, normal cycloid를 만들고
		// 싶어서 적당한 숫자를 찾던 도중에 해당 보정치가 가장 적당하게 값이 구해졌기 때문이다.
		a->setVector(radius*sin(radian), radius*cos(radian), 0);
		basic_kinematic(p, v, a, pt, dt);
	}

	void Kinematic::stair_up_kinematic(MyVector *&p, MyVector *&v, MyVector *&a, Particle & pt, float dt) {
		// dt를 정수형으로 캐스팅해서, 소수단위를 버리고 싸이클로이드 위치 벡터 방정식 p(t)를 속도벡터로 삼는 운동이다.
		// 마치 계단을 오르는 것 같은 운동을 한다.
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
		// 엄밀히 말하면, curtate cycloid의 일종이나 Trace mode를 켜고 관찰하면
		// 마치 3차원 공간의 Helix 진동을 보는 것 같아서 남겨두었다.
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
		// 키보드 화살표키로 직접 조종할 수 있는 강체에 대한 운동이다.
		//
		//
		p->setVector(pt.control_X, pt.control_Y, 0);
		basic_kinematic(p, v, a, pt, dt);
	}




