#pragma once
#define _CRT_SECURE_NO_WARNINGS		// 오래된 소프트웨어 경고
#define _USE_MATH_DEFINES
#define RigidList vector<Particle*>

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <time.h>

#include <windows.h>
#include <gl/freeglut.h>			// openGL을 계승하는 라이브러리 freeglut
#include "MyVector.cpp"
#include "particle.h"

using namespace std;


double control_X, control_Y;
int interface_bar_width = 400;
bool trace = false, trace_flag = false, guide_flag = false, kill = false, gravity = false, collide = true;
double tic = .0, elapsed_time = .0, radian = 10.0, process_speed = 78.0, mass = 1.0;
RigidList rigid_body_list;
RigidList::iterator it;
GLUquadricObj *target;
MyVector *prev_;
int kinematic_type = 0, rigid_color_type = 2;
char *kinematic_type_font[] = { "Drop","Circular","Pendulum","Cycloid","one_one","test 1", "test 2" };

int mouse_Trigger = 0;
int turn = 0, cushion = 0;
int mouse_x, mouse_y;
double power;
MyVector angle_dr;
Particle *p1_hit, *p1_white, *p2_hit, *p2_white, *current_rigid, *oppoir_rigid, *current_target, *oppoir_target;
int score = 3, phase = 1, p1_score = 0, p2_score = 0;
bool penalty_flag = false, oppoir_target_flag = false, current_target_flag = false, cushion_fail_flag = false;
int p1_pnum = 0, p2_pnum = 0, turn_num = 0;
double total_play_time;

void init();
void printString(int x, int y, int z, char *string);
void printBigString(int x, int y, int z, char *string);
Particle*& generate_Rigidbody(int x, int y, int z);
void draw_Interface();
void draw_Boundary();
void draw_Rigidbody();
void run_kinematic();
void boundary_kinematic(Particle *&p);
void gradation_value_set(Particle *&tmp, double&gradation, int c_type);
void rand_init();
int rand_interval(int start, int end);
void game_init();

void turn_Decision(int turn);
void angle_Decision();
void power_Decision();
void shoot_Decision();
void turn_end_Decision();
bool cushion_end_Decision();
void game_score_process(Particle *& p1, Particle *& p2);
void _3_cushion_process(Particle *& p1, Particle *& p2);
void cushion_check(Particle *&p);


void init() {
	// 기타 함수에 대한 초기설정
	prev_ = new MyVector(0, 0, 0);
	target = gluNewQuadric();
}

void printString(int x, int y, int z, char *string) {
	// 화면에 문자열을 출력해주는 함수
	glRasterPos2f(x, y);
	int len = (int)strlen(string);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
}

void printBigString(int x, int y, int z, char *string) {
	// 화면에 문자열을 출력해주는 함수
	glRasterPos2f(x, y);
	int len = (int)strlen(string);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

Particle*& generate_Rigidbody(int x, int y, int z, int id) {
	// x, y, z 위치에 강체를 생성한다.
	Particle *tmp = new Particle(mass, radian);
	tmp->setPosition(x, y, z);
	tmp->save_first_position(x, y, z);
	tmp->c_type = rigid_color_type;
	tmp->setKinematic(kinematic_type);
	tmp->id = id;
	rigid_body_list.push_back(tmp);

	return tmp;
}


int gap_generator(int const& cnt) {
	int interface_start = 300;
	int first_gap = 10;
	int content_gap = 30;
	if (cnt == 0) return interface_start - first_gap;
	return interface_start - first_gap - cnt*content_gap;
}


void change_Color(int type) {
	switch (type) {
	case 1:
		glColor3f(1., .0, .0);
		break;
	case 2:
		glColor3f(1., 1., 1.);
		break;
	case 3:
		glColor3f(1., .3, .3);
		break;
	case 4:
		glColor3f(.3, .3, .3);
		break;
	}
}

void init_Color() {
	glColor3f(.0, 1., .0);
}

void draw_Interface() {
	int cnt = 0;

	if (phase == 1) {
		// 화면에 인터페이스를 그리는 랜더링 함수
		glColor3d(.0, 1., .0);
		printString(400, gap_generator(cnt++), 0, " ** Score Select ** ");
		printString(400, gap_generator(cnt), 0, "current score : ");
		printString(500, gap_generator(cnt++), 0, (char*)to_string(score).c_str());
		//change_Color(rigid_color_type);
		//printString(600, gap_generator(cnt++), 0, (rigid_color_type == 1) ? "RED" : ((rigid_color_type == 2) ? "GREEN" : "BLUE"));
		//init_Color();
		printString(400, gap_generator(cnt), 0, "score  up  : q ");
		printString(500, gap_generator(cnt++), 0, "score down : w ");
		printString(400, gap_generator(cnt++), 0, "game starat : s ");
	}
	else if(phase == 2){
		glColor3d(.0, 1., .0);
		printString(400, gap_generator(cnt++), 0, " ** Score ** ");
		printString(400, gap_generator(cnt), 0, "player 1 score : ");
		printString(500, gap_generator(cnt++), 0, (p1_score==0) ? "3cushion mode" : (char*)(to_string(p1_score) + " / " + to_string(score)).c_str());

		printString(400, gap_generator(cnt), 0, "player 2 score : ");
		printString(500, gap_generator(cnt++), 0, (p2_score == 0) ? "3cushion mode" : (char*)(to_string(p2_score) + " / " + to_string(score)).c_str());

		printString(400, gap_generator(cnt++), 0, " ** Turn ** ");
		printString(400, gap_generator(cnt), 0, "current Player : ");
		printString(500, gap_generator(cnt++), 0, (turn==0) ? "player 1" : "player 2");
		printString(400, gap_generator(cnt), 0, "current Hit ball : ");
		printString(540, gap_generator(cnt++), 0, (turn==0) ? "RED" : "PINK");
		
		printString(400, gap_generator(cnt++), 0, " ** Phase ** ");
		printString(400, gap_generator(cnt), 0, "now Phase : ");

		switch (mouse_Trigger) {
		
		case 0 :
			printString(500, gap_generator(cnt++), 0, "Angle Decision ... ");
			printString(400, gap_generator(cnt++), 0, "(Left mouse button : Set the angle) ");
			break;

		case 1:
			printString(500, gap_generator(cnt++), 0, "Power Decision ... ");
			printString(400, gap_generator(cnt++), 0, "(Left mouse button : Set the Power) ");
			printString(400, gap_generator(cnt++), 0, "(Right mouse button : Back to the angle Decision) ");
			break;
		case 3:
			printString(500, gap_generator(cnt++), 0, "Ball Wait Shoot order ... ");
			printString(400, gap_generator(cnt), 0, "Power : ");
			printString(500, gap_generator(cnt++), 0, (char*)to_string(power).c_str());
			printString(400, gap_generator(cnt++), 0, "(Left mouse button : Shot) ");
			printString(400, gap_generator(cnt++), 0, "(Right mouse button : Back to the power Decision) ");
			break;
		case 5:
			printString(500, gap_generator(cnt++), 0, "Game processing ... ");
			if (penalty_flag) printString(400, gap_generator(cnt++), 0, " * you hit the opponent's hit ball : PENALTY ");
			if (oppoir_target_flag) printString(400, gap_generator(cnt++), 0, " * you hit the opponent's target ball ");
			if (current_target_flag) printString(400, gap_generator(cnt++), 0, " * you hit the own target ball ");
			if (current_target_flag&&oppoir_target_flag&&!penalty_flag) printString(400, gap_generator(cnt++), 0, " * you obtained score ");
			break;

		}


		cnt = 16;


		printString(400, gap_generator(cnt++), 0, " ** Option ** ");
		printString(400, gap_generator(cnt), 0, "guide Mode ( g ) : ");
		printString(500, gap_generator(cnt++), 0, (guide_flag == true) ? "ON" : "OFF");
		printString(400, gap_generator(cnt), 0, "Trace Mode ( t ) : ");
		printString(500, gap_generator(cnt++), 0, (trace_flag == true) ? "ON" : "OFF");
		printString(400, gap_generator(cnt), 0, "game Speed ( a/d ) : ");
		printString(540, gap_generator(cnt++), 0, (char*)to_string(process_speed).c_str());
	}
	else if (phase == 3) {
		cnt = 6;
		printBigString(400, gap_generator(cnt++), 0, " P L A Y E R 1 W I N !! ");
		printBigString(400, gap_generator(cnt++), 0, " CONGRATULATION !! ");

		cnt = 10;
		printString(400, gap_generator(cnt), 0, "Total Play Time : ");
		printString(500, gap_generator(cnt++), 0, (char*)to_string(total_play_time).c_str());
		printString(400, gap_generator(cnt), 0, "Total Turn Number : ");
		printString(600, gap_generator(cnt++), 0, (char*)to_string(turn_num).c_str());
		printString(400, gap_generator(cnt), 0, "Total Penalty Number : ");
		printString(600, gap_generator(cnt++), 0, (char*)to_string(p1_pnum).c_str());
		printString(400, gap_generator(cnt), 0, "press ' r ' key to restart the game");

	}
	else if (phase == 4) {
		cnt = 6;
		printBigString(400, gap_generator(cnt++), 0, " P L A Y E R 2 W I N !! ");
		printBigString(400, gap_generator(cnt++), 0, " CONGRATULATION !! ");

		cnt = 10;
		printString(400, gap_generator(cnt), 0, "Total Play Time : ");
		printString(500, gap_generator(cnt++), 0, (char*)to_string(total_play_time).c_str());
		printString(400, gap_generator(cnt), 0, "Total Turn Number : ");
		printString(600, gap_generator(cnt++), 0, (char*)to_string(turn_num).c_str());
		printString(400, gap_generator(cnt), 0, "Total Penalty Number : ");
		printString(600, gap_generator(cnt++), 0, (char*)to_string(p2_pnum).c_str());
		printString(400, gap_generator(cnt), 0, "press ' r ' key to restart the game");

	}

}

void draw_Boundary() {
	// 전체 화면은 800 * 800, 강체들이 튕길 경계면은
	// 내부의 600 * 600 공간이다.
	glColor3d(.0, 1., .0);
	glBegin(GL_LINE_LOOP);
	glVertex3d(-300, -300, 0);
	glVertex3d(-300, 300, 0);
	glVertex3d(300, 300, 0);
	glVertex3d(300, -300, 0);
	glEnd();

}

void draw_Rigidbody() {
	// 화면에 강체들을 전부 그리는 랜더링 함수
	Particle *tmp;
	MyVector *pos;
	for (it = rigid_body_list.begin(); it != rigid_body_list.end(); it++) {
		tmp = *it;
		pos = tmp->getPosition();
		gradation_value_set(tmp, tmp->gradation, tmp->c_type);
		glPushMatrix();
		glTranslated(pos->getX(), pos->getY(), pos->getZ());
		gluSphere(target, tmp->getRadius(), 10, 10);
		glPopMatrix();
		init_Color();
	}
}


void collide_check() {
	RigidList::iterator target;

	for (int i = 0; i < rigid_body_list.size(); i++) {
		for (target = rigid_body_list.begin() + i + 1, it = rigid_body_list.begin() + i ; target != rigid_body_list.end(); target++) {
		
			Particle *p1 = *it;
			Particle *p2 = *target;

			if (p1 == p2) continue;

			if (p1->getPosition()->getEuclidean(*p2->getPosition()) < p1->getRadius() + p2->getRadius()) {
				// 두 강체의 거리가 두 강체의 반경 합보다 작다면 그것은 충돌임.

				double m1 = p1->mass;
				double m2 = p2->mass;

				double r1 = p1->getRadius();
				double r2 = p2->getRadius();


				MyVector ein = *p2->getPosition() + *p1->getPosition()*-1.;
				// 두 강체의 중심을 연결한 벡터.

				double coss1 = p1->getVelocity()->getScale() == 0 ? 0 : ein * (*p1->getVelocity()) / (ein.getScale()*p1->getVelocity()->getScale());
				double coss2 = p2->getVelocity()->getScale() == 0 ? 0 : ein*-1. * (*p2->getVelocity()) / (ein.getScale()*p2->getVelocity()->getScale());

				// 해당 객체 기준으로 벡터 성분 분해
				MyVector u1x = ein * (1. / ein.getScale())*p1->getVelocity()->getScale()*coss1;

				// 충돌 대상 객체 기준으로 벡터 성분 분해
				MyVector u2x = ein * (-1. / ein.getScale())*p2->getVelocity()->getScale()*coss2;



				// 충돌
				MyVector v1x = (u1x*(m1 - m2) + u2x*m2*2.)*(1. / (m1 + m2));
				MyVector v2x = (u2x*(m2 - m1) + u1x*m1*2.)*(1. / (m1 + m2));

				// 가속도 제거
				//p1->getAcceleration()->setZero();
				//p2->getAcceleration()->setZero();

				// 신규 속도 지정
				MyVector v1_after = v1x + u1x*-1. + *p1->getVelocity();
				MyVector v2_after = v2x + u2x*-1. + *p2->getVelocity();
				p1->getVelocity()->setVector(v1_after);
				p2->getVelocity()->setVector(v2_after);


				double ein_Scale1 = 1.01*abs(p1->getRadius() - ein.getScale());
				double ein_Scale2 = 1.01*abs(p2->getRadius() - ein.getScale());

				MyVector p1_after = *p1->getPosition() + *(p1->getVelocity())*(ein_Scale1 / p1->getVelocity()->getScale());
				MyVector p2_after = *p2->getPosition() + *(p2->getVelocity())*(ein_Scale2 / p2->getVelocity()->getScale());
				//p1->getPosition()->setVector(p1_after);
				//p2->getPosition()->setVector(p2_after);




				if ((turn == 0 && p1_score == 0)||(turn == 1 && p2_score == 0)) {
					_3_cushion_process(p1, p2);
				}
				else {
					game_score_process(p1, p2);
				}
			}

		}

	}
}


void game_score_process(Particle *& p1, Particle *& p2) {


	if (p1 == oppoir_rigid || p2 == oppoir_rigid) penalty_flag = true;
	if (p1 == current_target || p2 == current_target) current_target_flag = true;
	if (p1 == oppoir_target || p2 == oppoir_target) oppoir_target_flag = true;


}

void _3_cushion_process(Particle *& p1, Particle *& p2) {

	if (p1 == p1_hit && p2 == p2_hit) cushion_fail_flag = true;
	if ((p1 == current_target || p2 == current_target)) {
		if(cushion < 3) cushion_fail_flag = true;
		current_target_flag = true;
	}
	if ((p1 == oppoir_target || p2 == oppoir_target)) {
		if (cushion < 3) cushion_fail_flag = true;
		oppoir_target_flag = true;
	}

}

void run_kinematic() {
	// 각 강체의 운동을 진행시키는 함수
	// 
	double sensitive_run = 50.;
	// 충돌 검출을 정확하게 하기 위해, 각 운동을 20 조각으로 나눠서 수행한다는 의미.
	//
	for (double i = 0.; i < sensitive_run; i += 1.) {

		Particle *tmp;
		for (it = rigid_body_list.begin(); it != rigid_body_list.end(); it++) {
			tmp = *it;
			if (tmp->getKinematic() == 4) tmp->setControl(control_X, control_Y);
			tmp->gravity_flag = gravity;
			tmp->run(1. / sensitive_run);
			boundary_kinematic(tmp);
		}
		if (collide && rigid_body_list.size() > 1) collide_check();
	}
}



void boundary_kinematic(Particle *&p) {
	double radius = p->getRadius();
	MyVector *pos = p->getPosition();
	MyVector *vel = p->getVelocity();

	double decrease = 1.; //conflict_decrease ? 1. : 1.;

	if (pos->getY() + radius >= 300. || pos->getY() - radius <= -300.) {
		vel->setY(-1.*decrease* vel->getY());
		if (pos->getY() + radius >= 300.)
			pos->setY(2 * (300. - radius) - pos->getY());
		if (pos->getY() - radius <= -300.)
			pos->setY(2 * (-300. + radius) - pos->getY());
		cushion_check(p);
	}
	if (pos->getX() + radius >= 300. || pos->getX() - radius <= -300.) {
		vel->setX(-1.*decrease* vel->getX());
		if (pos->getX() + radius >= 300.)
			pos->setX(2 * (300. - radius) - pos->getX());
		if (pos->getX() - radius <= -300.)
			pos->setX(2 * (-300. + radius) - pos->getX());
		cushion_check(p);
	}
}


void cushion_check(Particle *&p){
	if ((p == current_rigid&&turn == 0 && p1_score == 0) || (p == current_rigid&&turn == 1 && p2_score == 0)) cushion++;
}


void gradation_value_set(Particle *&tmp, double&gradation, int c_type) {
	
	if (!(tmp->getVelocity()->getScale()!=0 && trace)) {
		gradation = 0;
		switch (c_type) {
		case 1:
			glColor3f(1., .0, .0);
			break;
		case 2:
			glColor3f(1., 1., 1.);
			break;
		case 3:
			glColor3f(1., .3, .3);
			break;
		case 4:
			glColor3f(.3, .3, .3);
			break;;
		}
	}
	else {
		gradation += 0.0025;
		switch (c_type) {
		case 1:
			glColor3f(gradation, .0, .0);
			break;
		case 2:
			glColor3f(gradation, gradation, gradation);
			break;
		case 3:
			glColor3f(gradation, .3*gradation, .3*gradation);
			break;
		case 4:
			glColor3f(.3*gradation, .3*gradation, .3*gradation);
			break;
		}
	}
}



void display()
{

	if (!trace) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Trace Mode가 아니라면 z버퍼, 컬러버퍼를 초기화 한다.
	}
	else {
		glScissor(800, 0, 400, 800);
		// 인터페이스의 좌표
		glEnable(GL_SCISSOR_TEST);
		// 인터페이스는 Trace Mode에 의해 새로 쓰여질 이유가 없으므로 Scissor를 이용해서
		// 항상 최신화한다.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);
		// Scissor에 의해서 인터페이스 부분만 지워지고 최신값을 랜더링한다.
		glClear(GL_DEPTH_BUFFER_BIT);
		// 이후, 전체 버퍼에 대해서 z버퍼만 초기화 하는 것으로 이전 버퍼의 컬러버퍼 흔적을 남겨
		// 경로가 추적되는 묘사를 할 수 있다.
	}

	glMatrixMode(GL_MODELVIEW);
	
	draw_Interface();
	draw_Boundary();
	draw_Rigidbody();

	if (phase == 2) {
		turn_Decision(turn);
		angle_Decision();
		power_Decision();
		shoot_Decision();
		turn_end_Decision();
	}


	elapsed_time += ((double)glutGet(GLUT_ELAPSED_TIME) / 1000. - tic);
	if (elapsed_time < 1. / process_speed) {
	}
	else {
		elapsed_time = 0.;
		run_kinematic();
	}


	// 더블버퍼링
	glutSwapBuffers();
	tic = (double)glutGet(GLUT_ELAPSED_TIME) / 1000.;
}

void keyboard(unsigned char key, int x, int y) {
	// 키보드 이벤트 핸들링은 여기
	switch (key) {

	case 'r':
		p1_pnum = p2_pnum = turn_num = 0;
		phase = 1;
		score = 3;
		rigid_body_list.clear();
		mouse_Trigger = 0;
		turn = 0;
		p1_score = p2_score = 0;
		penalty_flag = oppoir_target_flag = current_target_flag = false;
		guide_flag = cushion_fail_flag = trace_flag = false;
		cushion = 0;
		break;
	case 's':
		if(phase==1) game_init();
		break;
	case 'q':
		if (phase == 1) score += 1;
		break;
	case 'w':
		if (score>1 && phase == 1) score -= 1;
		break;
	case '0':
		p1_score = 0;
		break;
	case '1':
		phase = 3;
		break;

	case 'a':
		process_speed += 1.;
		break;
	case 'd':
		process_speed -= 1.;
		if (process_speed <= 1.) process_speed = 0.1;
		break;

	case 't':
		trace_flag = !trace_flag;
		break;
	case 'g':
		guide_flag = !guide_flag;
		break;

	}
}

void mousemove(int x, int y) {

	if (x<700 && x>100)	mouse_x = x - 400;
	if (y<700 && y>100)	mouse_y = 400 - y;

}

void mousepress(int button, int state, int x, int y) {
	// 마우스 이벤트 핸들링은 여기

	if (!(mouse_Trigger > -1 && mouse_Trigger < 5) || phase!=2) return;

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		mouse_Trigger++;
	}
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
		mouse_Trigger=0;
	}

	mousemove(x, y);

}



void turn_Decision(int turn) {
	
	switch (turn) {
	case 0 :
		current_rigid = p1_hit;
		oppoir_rigid = p2_hit;
		current_target = p1_white;
		oppoir_target = p2_white;
		break;
	case 1 :
		current_rigid = p2_hit;
		oppoir_rigid = p1_hit;
		current_target = p2_white;
		oppoir_target = p1_white;
		break;

	}
}

void angle_Decision() {
	if(mouse_Trigger==1)
	angle_dr = MyVector(mouse_x,mouse_y,0) + *current_rigid->getPosition()*-1.;
	angle_dr = angle_dr * (75.0 / angle_dr.getScale());
	if (guide_flag && mouse_Trigger < 4) {
		glColor3d(1.0, 1., .0);
		glBegin(GL_LINE_STRIP);
		glVertex3d(current_rigid->getPosition()->getX(), current_rigid->getPosition()->getY(), current_rigid->getPosition()->getZ());
		glVertex3d(angle_dr.getX() + current_rigid->getPosition()->getX(), angle_dr.getY() + current_rigid->getPosition()->getY(), angle_dr.getZ() + current_rigid->getPosition()->getZ());
		glEnd();
	}
}

void power_Decision() {
	if (mouse_Trigger == 2) {
		power = MyVector(mouse_x, mouse_y, 0).getEuclidean(*current_rigid->getPosition()) / 7.;
		mouse_Trigger++;

	}
}

void shoot_Decision() {
	if (mouse_Trigger == 4) {
		current_rigid->getVelocity()->setVector(angle_dr*power);
		mouse_Trigger++;
		if(trace_flag) trace = true;
	}
}

void turn_end_Decision() {
	if (mouse_Trigger == 5 && p1_hit->getVelocity()->getScale()==0 && p1_white->getVelocity()->getScale() == 0 && p2_hit->getVelocity()->getScale() == 0 && p2_white->getVelocity()->getScale() == 0 ) {
		turn_num++;
		if (cushion_end_Decision()) { 
			total_play_time = ((double)glutGet(GLUT_ELAPSED_TIME) / 1000.);
			return; 
		}
		
		if (current_target_flag&&oppoir_target_flag && !penalty_flag) (turn == 0) ? p1_score-- : p2_score--;
		if (penalty_flag) (turn == 0) ? p1_score++ , p1_pnum++ : p2_score++ , p2_pnum++;
	
		if (p1_score < 0) p1_score = 0;
		if (p2_score < 0) p2_score = 0;

		mouse_Trigger=0;
		power = 0;
		cushion = 0;
		turn ^= 1;
		oppoir_target_flag = penalty_flag = current_target_flag = cushion_fail_flag = trace = false;


	}
}

bool cushion_end_Decision() {
	if (oppoir_target_flag && current_target_flag) {
		if (turn == 0 && cushion_fail_flag == false && p1_score == 0) {
			phase = 3;
			return true;
		}
		if (turn == 1 && cushion_fail_flag == false && p2_score == 0) {
			phase = 4;
			return true;
		}
	}
	return false;
}


void rand_init() {
	srand((unsigned int)time(NULL));
}

int rand_interval(int start, int end) {
	return rand() % (end-start) + start;
}


void game_init() {
	phase++;
	p1_score = p2_score = score;
	rand_init();
	rigid_color_type = 1;
	p1_hit = generate_Rigidbody(rand_interval(100,700) - 400, 400 - rand_interval(100, 700),0,1);
	rigid_color_type = 2;
	p1_white = generate_Rigidbody(rand_interval(100, 700) - 400, 400 - rand_interval(100, 700), 0,2);
	rigid_color_type = 3;
	p2_hit = generate_Rigidbody(rand_interval(100, 700) - 400, 400 - rand_interval(100, 700), 0,3);
	rigid_color_type = 4;
	p2_white = generate_Rigidbody(rand_interval(100, 700) - 400, 400 - rand_interval(100, 700), 0,4);
}


void callBackHere() {
	//openGL 콜백함수 레지스터 함수는 여기에 모아둔다.
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousepress);
	glutMotionFunc(mousemove);
}




int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(800 + interface_bar_width, 800);
	glutCreateWindow("4 billiard game");
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-400, 400 + interface_bar_width, -400, 400, -100.0, 100.0);
	gluLookAt(0, 0, 50, 0, 0, -1, 0, 1, 0);

	glViewport(0, 0, 800 + interface_bar_width, 800);
	init();
	callBackHere();
	glutMainLoop();

	return 0;
}



