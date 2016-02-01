#pragma once
#include "matrix.h"
#define NEGATIVE_BALL 1
#define POSITIVE_BALL 2
#define SPEED_INC  50
class Ball
{
public:
	int type;
	double radius;
	double friction;
	double maxSpeed;
	double speed;
	bool isChosed;
	Color4f color;
	Vector3d position;
	Vector3d direction;
	void crashBox(double left,double top,double right,double bottom,double time);
	void crashBall(Ball &ball,double time);
	void move(double time);
	Vector3d nextpos(double time);
	void draw();
	Ball(double mRadius,double mFriction,double mSpeed, Vector3d mDirection,Vector3d mPosition,Color4f mColor,int mType);
	~Ball();

private:

};
