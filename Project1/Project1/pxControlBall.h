#pragma once

#include"pxBall.h"
#include"camera.h"
#include"particle.h"
class PxControlBall : public PxBall {
public:
	void move(int front, int right,Camera &camera);
	void jump();
	void draw();//º”»Îæ€π‚µ∆
	void PxControlBall::addJmpCPSS();
	PxControlBall(Color4f c, GLfloat r);
};