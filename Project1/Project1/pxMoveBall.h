#pragma once
#include"pxBall.h"
class PxMoveBall:public PxBall{
public:
	PxReal maxspeed;
	void move();
	PxMoveBall(Color4f c, GLfloat r,PxReal maxspeed);
};
