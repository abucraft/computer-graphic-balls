#pragma once
#include<gl/glew.h>
#include<GL\glut.h>
#include<math.h>
#define A2R(a) ((a/180.0)*3.1415926)
typedef GLfloat Point[3];
typedef GLfloat UVPoint[2];
typedef GLfloat NMPoint[3];
typedef GLfloat TGPoint[3];
typedef GLfloat CLPoint[4];
struct Color4f
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
	Color4f();
	Color4f(GLfloat r, GLfloat g, GLfloat b);
	Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
};

void SetMaterialColor(Color4f color4f, GLenum face = GL_FRONT, GLenum type = GL_AMBIENT_AND_DIFFUSE);

struct Vector3f
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	Vector3f();
	Vector3f(GLfloat gx, GLfloat gy, GLfloat gz);
	void normalize();
};

struct Vector3d
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	void normalize();
	double length();
	Vector3d();
	Vector3d(GLdouble, GLdouble, GLdouble);
};

Vector3d cross(Vector3d& left, Vector3d& right);
Vector3f cross(Vector3f& left, Vector3f& right);
Vector3d operator-(Vector3d&left, Vector3d&right);
Vector3f operator-(Vector3f&left, Vector3f&right);
double operator*(Vector3d&left, Vector3d&right);
float operator*(Vector3f&left, Vector3f&right);
Vector3d operator-(Vector3d&right);
Vector3f operator-(Vector3f&right);
Vector3d operator*(Vector3d&left, double right);
Vector3f operator*(Vector3f&left, double right);
Vector3d operator+(Vector3d&left, Vector3d&right);
Vector3f operator+(Vector3f&left, Vector3f&right);
struct Quaternion4d
{
	GLdouble s;
	GLdouble x;
	GLdouble y;
	GLdouble z;
	void setPoint(Vector3d& p);
	void setRotation(double angle, Vector3d& axis);
};
Vector3d rotate(Vector3d& axis, Vector3d& position,double radian,Vector3d&point);
Quaternion4d operator*(Quaternion4d& left, Quaternion4d right);

struct Vector2f
{
	GLfloat x;
	GLfloat y;
	void normalize();
};

struct Vector2d {
	GLdouble x;
	GLdouble y;
	void normalize();
};

struct Vector2i {
	int x;
	int y;
	void normalize();
};