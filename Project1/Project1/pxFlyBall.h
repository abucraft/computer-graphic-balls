#pragma once
#include"pxBall.h"
#include"particle.h"
class PxFlyBall : public PxBall {
public:
	int time;
	int particleTime;
	void move();
	void addCPSS();
	PxFlyBall(Color4f c, GLfloat r);
};

