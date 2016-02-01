#pragma once
#include"3DObject.h"
#include<PxPhysics.h>
#include<PxPhysicsAPI.h>
#include"matrix.h"
using namespace physx;
class PxBall {
public:
	Material mtl;
	Color4f color;
	PxRigidDynamic *pxActor;
	GLfloat radius;
	GLUquadricObj *sphere;
	void draw();
	void drawWithMtl();
	void setMaterial(Material mtl);
	bool createPxBall(PxPhysics& physics,const PxTransform& t,PxMaterial& mtl);
	PxBall(Color4f c,GLfloat r);
	~PxBall();
};