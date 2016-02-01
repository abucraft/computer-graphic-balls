#pragma once
#include<stdlib.h>
#include"ball.h"
#define FLY_BALL	3
#define FLY			1
#define REST		2	
#define GO_FLY		3
#define GO_REST		4
class FlyBall: public Ball
{
public:
	double livingTime;
	int state;
	void move(double time);
	void crashBall(Ball &ball, double time);
	FlyBall(double mRadius, double mFriction, double mSpeed, Vector3d mDirection, Vector3d mPosition, Color4f mColor);
	~FlyBall();

private:

};
