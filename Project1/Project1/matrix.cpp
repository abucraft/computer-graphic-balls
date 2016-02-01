#include "matrix.h"

void SetMaterialColor(Color4f color4f, GLenum face, GLenum type) {
	GLfloat colorfv[4];
	colorfv[0] = color4f.red;
	colorfv[1] = color4f.green;
	colorfv[2] = color4f.blue;
	colorfv[3] = color4f.alpha;
	glMaterialfv(face, type, colorfv);
}

Vector3f::Vector3f()
{
}

Vector3f::Vector3f(GLfloat gx, GLfloat gy, GLfloat gz):x(gx),y(gy),z(gz)
{
}

void Vector3f::normalize()
{
	GLdouble length = sqrt(x*x + y*y + z*z);
	x = x / length;
	y = y / length;
	z = z / length;
}

void Vector3d::normalize()
{
	GLdouble length = sqrt(x*x + y*y + z*z);
	if (length == 0.0)
		return;
	x = x / length;
	y = y / length;
	z = z / length;
}

double Vector3d::length()
{
	return sqrt(x*x+y*y+z*z);
}

Vector3d::Vector3d()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3d::Vector3d(GLdouble x, GLdouble y, GLdouble z):x(x),y(y),z(z)
{
}

Vector3d cross(Vector3d& left, Vector3d& right) {
	Vector3d result;
	result.x = left.y*right.z - left.z*right.y;
	result.y = left.z*right.x - left.x*right.z;
	result.z = left.x*right.y - left.y*right.x;
	return result;
}

Vector3f cross(Vector3f& left, Vector3f& right) {
	Vector3f result;
	result.x = left.y*right.z - left.z*right.y;
	result.y = left.z*right.x - left.x*right.z;
	result.z = left.x*right.y - left.y*right.x;
	return result;
}

Vector3d operator-(Vector3d&left, Vector3d&right) {
	Vector3d result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;
	return result;
}

Vector3f operator-(Vector3f&left, Vector3f&right) {
	Vector3f result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;
	return result;
}

Vector3d operator-(Vector3d&right) {
	Vector3d result;
	result.x = -right.x;
	result.y = -right.y;
	result.z = -right.z;
	return result;
}

Vector3f operator-(Vector3f&right) {
	Vector3f result;
	result.x = -right.x;
	result.y = -right.y;
	result.z = -right.z;
	return result;
}

Vector3d operator*(Vector3d&left, double right) {
	Vector3d result;
	result.x = left.x*right;
	result.y = left.y*right;
	result.z = left.z*right;
	return result;
}

Vector3f operator*(Vector3f&left, double right) {
	Vector3f result;
	result.x = left.x*right;
	result.y = left.y*right;
	result.z = left.z*right;
	return result;
}


Vector3d operator+(Vector3d&left, Vector3d&right) {
	Vector3d result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;
	return result;
}

Vector3f operator+(Vector3f&left, Vector3f&right) {
	Vector3f result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;
	return result;
}

double operator*(Vector3d&left, Vector3d&right) {
	return left.x*right.x + left.y*right.y + left.z*right.z;
}

float operator*(Vector3f&left, Vector3f&right) {
	return left.x*right.x + left.y*right.y + left.z*right.z;
}

Vector3d rotate(Vector3d& axis, Vector3d& position,double radian, Vector3d&point) {
	Quaternion4d rotate, backrotate,qdest;
	Vector3d dest;
	axis.normalize();
	rotate.setRotation(radian, axis);
	backrotate.setRotation(radian, -axis);
	dest = point - position;
	qdest.setPoint(dest);
	qdest = rotate*qdest*backrotate;
	dest.x = qdest.x;
	dest.y = qdest.y;
	dest.z = qdest.z;
	dest = dest + position;
	return dest;
}


Quaternion4d operator*(Quaternion4d& left, Quaternion4d right) {
	Quaternion4d result;
	double d1, d2, d3, d4;

	d1 = left.s * right.s;
	d2 = -left.x * right.x;
	d3 = -left.y * right.y;
	d4 = -left.z * right.z;
	result.s = d1 + d2 + d3 + d4;

	d1 = left.s * right.x;
	d2 = right.s * left.x;
	d3 = left.y * right.z;
	d4 = -left.z * right.y;
	result.x = d1 + d2 + d3 + d4;

	d1 = left.s * right.y;
	d2 = right.s * left.y;
	d3 = left.z * right.x;
	d4 = -left.x * right.z;
	result.y = d1 + d2 + d3 + d4;

	d1 = left.s * right.z;
	d2 = right.s * left.z;
	d3 = left.x * right.y;
	d4 = -left.y * right.x;
	result.z = d1 + d2 + d3 + d4;

	return result;
}

void Vector2f::normalize()
{
	GLdouble length = sqrt(x*x + y*y);
	x = x / length;
	y = y / length;
}

void Vector2d::normalize()
{
	GLdouble length = sqrt(x*x + y*y);
	x = x / length;
	y = y / length;
}

void Vector2i::normalize()
{
	GLdouble length = sqrt(x*x + y*y);
	x = x / length;
	y = y / length;
}

void Quaternion4d::setPoint(Vector3d&p)
{
	s = 0;
	x = p.x;
	y = p.y;
	z = p.z;
}

void Quaternion4d::setRotation(double angle, Vector3d& axis)
{
	s = cos(angle / 2);
	double sinAngle = sin(angle / 2);
	x = axis.x*sinAngle;
	y = axis.y*sinAngle;
	z = axis.z*sinAngle;
}

Color4f::Color4f() :red(0), green(0), blue(0), alpha(1)
{
}

Color4f::Color4f(GLfloat r, GLfloat g, GLfloat b):red(r),green(g),blue(b),alpha(1)
{
}

Color4f::Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) : red(r), green(g), blue(b), alpha(a)
{
}
