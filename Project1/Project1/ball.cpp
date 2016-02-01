#include"ball.h"

void Ball::crashBox(double left, double top, double right, double bottom, double time)
{
	Vector3d nextp = position + direction*(speed*time);
	if (left - position.x < radius&&left - position.x > -radius) {
		position.x = left + radius;
	}
	if (right - position.x < radius && right - position.x > -radius) {
		position.x = right - radius;
	}
	if (top - position.z < radius && top - position.z > -radius) {
		position.z = top + radius;
	}
	if (bottom - position.z < radius && bottom - position.z > -radius) {
		position.z = bottom - radius;
	}
	if ((left - nextp.x < radius&&left-nextp.x> -radius) || (right - nextp.x < radius && right - nextp.x > -radius)) {
		direction.x = -direction.x;
	}
	if ((top - nextp.z < radius && top-nextp.z > -radius) || (bottom - nextp.z < radius && bottom - nextp.z > -radius)) {
		direction.z = -direction.z;
	}
}

void Ball::crashBall(Ball & ball,double time)
{
	Vector3d posi = position + direction*(speed*time);
	Vector3d posj = ball.nextpos(time);
	Vector3d pointTo = posj - posi;
	if (pointTo.length() <= radius + ball.radius) {
		int speedi = speed;
		int speedj = ball.speed;
		Vector3d p2n = pointTo;
		p2n.normalize();
		Vector3d spVi = direction*speedi;
		Vector3d spVj = ball.direction*speedj;
		Vector3d spVin = p2n*(spVi*p2n);
		Vector3d spViv = spVi - spVin;
		Vector3d spVjn = p2n*(spVj*p2n);
		Vector3d spVjv = spVj - spVjn;
		Vector3d newspVi = spVjn + spViv;
		Vector3d newspVj = spVin + spVjv;
		speed = newspVi.length();
		direction = newspVi;
		direction.normalize();
		ball.speed = newspVj.length();
		ball.direction = newspVj;
		ball.direction.normalize();
	}
	pointTo = position - ball.position;
	if (pointTo.length() < (radius + ball.radius)/1.1) {
		double delt = (radius + ball.radius - pointTo.length())/2;
		Vector3d p2n = pointTo;
		p2n.normalize();
		position = position + p2n*-delt;
		ball.position = ball.position + p2n*delt;
	}
}

void Ball::move(double time)
{
	position = position + direction*(speed*time);
	if (type == NEGATIVE_BALL) {
		if (speed > 0) {
			speed -= friction*time;
		}
		else
		{
			speed = 0;
		}
	}
	if (type == POSITIVE_BALL) {
		if (speed < maxSpeed) {
			speed += SPEED_INC*time;
		}
		else
		{
			speed = maxSpeed;
		}
	}
}

Vector3d Ball::nextpos(double time)
{
	return position + direction*(speed*time);
}

void Ball::draw()
{
	glTranslated(position.x, position.y, position.z);
	GLfloat colorfv[4];
	if (isChosed) {
		glColor3f(color.red - 0.5, color.green - 0.5, color.blue - 0.5);
		SetMaterialColor(Color4f(color.red-0.5,color.green-0.5,color.blue-0.5));
		SetMaterialColor(color,GL_FRONT,GL_SPECULAR);
	}
	else {
		glColor3f(color.red, color.green, color.blue);
		SetMaterialColor(color, GL_FRONT, GL_SPECULAR);
		SetMaterialColor(color);
	}
	if (type == NEGATIVE_BALL) {
		glutSolidSphere(radius, 20, 20);
	}
	if (type == POSITIVE_BALL) {
		glutWireSphere(radius, 20, 20);
	}
	glTranslated(-position.x, -position.y, -position.z);
}

Ball::Ball(double mRadius, double mFriction, double mSpeed, Vector3d mDirection, Vector3d mPosition, Color4f mColor, int mType)
{
	isChosed = false;
	radius = mRadius;
	friction = mFriction;
	maxSpeed = mSpeed;
	speed = 0;
	direction = mDirection;
	direction.normalize();
	position = mPosition;
	color = mColor;
	type = mType;
}

Ball::~Ball()
{
}
