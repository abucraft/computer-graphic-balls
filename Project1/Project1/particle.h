#pragma once
#include<random>
#include"matrix.h"
#include"camera.h"
#define CIRCLE_PARTICLE_NUM 10
#define CIRCLE_PARTICLESYS_NUM 2000
#define MIN_FLOAT 0.0000000000001f
#define PI 3.1415926f
class Particle {
public:
	Vector3f location;
	Vector3f velocity;
	GLfloat size;
	bool dead;
	void move(GLfloat time);
	GLfloat maxlife;
	GLfloat lifetime;
	GLfloat decline;
	GLfloat accelerate;
	Particle();
};

class CircleParticle:public Particle{
public:
	GLfloat angle;
	GLfloat angleDump;
	GLfloat angleSpeed;
	GLfloat radius;
	GLfloat radiusChange;
	void move(GLfloat time);
	Vector3f getLocation();
};


class CircleParticleSys {
public:
	GLuint texId;
	Color4f color;
	bool dead;
	Camera*camera;
	CircleParticle particles[CIRCLE_PARTICLE_NUM];
	void init(GLfloat size, Color4f color, Vector3f location, Vector3f minVelocity,Vector3f maxVelocity, GLfloat diverge, GLfloat lifetime, GLfloat minDecline, GLfloat maxDecline, GLfloat minAccelerate,GLfloat maxAccelerate);
	void initCircle(GLfloat minRadius,GLfloat maxRadius,GLfloat minRadiusChange,GLfloat maxRadiusChange, GLfloat minAngleSpeed, GLfloat maxAngleSpeed, GLfloat minAngleDump,GLfloat maxAngleDump);
	void simulate(GLfloat time);
	void draw();
	bool isDead();
	CircleParticleSys();
};

void initCPSS(Camera *camera,GLuint texID);
void drawCPSS();
void simulateCPSS(GLfloat time);