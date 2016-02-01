#include"flyBall.h"

void FlyBall::move(double time)
{
	Ball::move(time);
	livingTime += time;
	if (state == REST) {
		if ((int)livingTime % 30 == 0) {
			state = GO_FLY;
			direction.y = 0.5;
			if (direction.x == 0 && direction.z==0) {
				direction.x = (rand() % 100) / 100.0;
				direction.z = (rand() % 100) / 100.0;
			}
			speed = maxSpeed;
			direction.normalize();
		}
	}
	if (state == GO_FLY) {
		if ((int)livingTime % 30 > 5) {
			state = FLY;
			direction.y += rand() % 100 / 100.0 - 0.5;
			direction.x += rand() % 100 / 100.0 - 0.5;
			direction.z += rand() % 100 / 100.0 - 0.5;
			speed += rand() % 100 / 10.0 - 5;
			direction.normalize();
		}
	}
	if (state == FLY) {
		if ((int)livingTime % 30 > 10) {
			state = GO_REST;
			direction.y = -0.5;
			direction.x = (rand() % 100) / 100.0;
			direction.z = (rand() % 100) / 100.0;
			direction.normalize();
		}
		else
		{
			if (position.y - 3*radius < 1) {
				direction.y = 0.2;
			}
			direction.y += rand() % 10 / 100;
			direction.normalize();
		}

	}
	if (state == GO_REST) {
		if (position.y - radius < 1) {
			direction.y = 0;
			position.y = radius;
			direction.normalize();
			state = REST;
		}
		else {
		}
	}
}

void FlyBall::crashBall(Ball & ball, double time)
{
	if (state == REST) {
		Ball::crashBall(ball, time);
	}
}

FlyBall::FlyBall(double mRadius, double mFriction, double mSpeed, Vector3d mDirection, Vector3d mPosition, Color4f mColor):Ball(mRadius, mFriction, mSpeed, mDirection, mPosition, mColor, NEGATIVE_BALL)
{
	state = REST;
	livingTime = 0;
}

FlyBall::~FlyBall()
{
}