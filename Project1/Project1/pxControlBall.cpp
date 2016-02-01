#include "pxControlBall.h"

void PxControlBall::move(int front, int right,Camera &camera)
{
	Vector3d camTo = camera.center - camera.position;
	Vector3d camCross = cross(camTo,camera.up);
	camTo.normalize();
	camCross.normalize();
	pxActor->addForce(PxVec3(camTo.x*front * 200, 0, camTo.z*front * 200), PxForceMode::eACCELERATION);
	pxActor->addForce(PxVec3(camCross.x*right*200, 0, camCross.z*right * 200), PxForceMode::eACCELERATION);
}

void PxControlBall::jump()
{
	pxActor->addForce(PxVec3(0, 120, 0.0), PxForceMode::eVELOCITY_CHANGE);
	addJmpCPSS();
}

void PxControlBall::draw()
{
	
	PxBall::drawWithMtl();

}

void PxControlBall::addJmpCPSS()
{
	PxVec3 v = pxActor->getLinearVelocity();
	PxTransform trans = pxActor->getGlobalPose();
	PxVec3 pos = trans.p;
	PxVec3 vn = v.getNormalized();
	extern GLuint pCPS;
	extern CircleParticleSys circleParticleSyses[CIRCLE_PARTICLESYS_NUM];
	Color4f white(0.8, 0.8, 0.8, 1.0);
	GLuint tgt = pCPS%CIRCLE_PARTICLESYS_NUM;
	Vector3f pvn(0, -1, 0);
	circleParticleSyses[tgt].init(3, white, Vector3f(pos.x, pos.y-4, pos.z), pvn * 0.2, pvn * 0.4, 0, 1, 1, 2, 1.2, 1.4);
	circleParticleSyses[tgt].initCircle(8, 10, 1.8, 2, PI / 100, PI / 200, 0.8, 1);
	pCPS = pCPS + 1;

	

	tgt = pCPS%CIRCLE_PARTICLESYS_NUM;
	circleParticleSyses[tgt].init(2.5, white, Vector3f(pos.x, pos.y-2, pos.z), pvn * 0.2, pvn * 0.4, 0, 1, 1, 2, 1.2, 1.4);
	circleParticleSyses[tgt].initCircle(7, 9, 1.8, 2, PI / 100, PI / 200, 0.8, 1);
	pCPS = pCPS + 1;

	tgt = pCPS%CIRCLE_PARTICLESYS_NUM;
	circleParticleSyses[tgt].init(2.5, white, Vector3f(pos.x, pos.y - 2, pos.z), pvn * 0.2, pvn * 0.4, 0, 1, 1, 2, 1.2, 1.4);
	circleParticleSyses[tgt].initCircle(7, 9, 1.8, 2, PI / 100, PI / 200, 0.8, 1);
	pCPS = pCPS + 1;

	tgt = pCPS%CIRCLE_PARTICLESYS_NUM;
	circleParticleSyses[tgt].init(2, white, Vector3f(pos.x, pos.y, pos.z), pvn * 0.2, pvn * 0.4, 0, 1, 1, 2, 1.2, 1.4);
	circleParticleSyses[tgt].initCircle(5, 6, 1.8,2, PI / 100, PI / 200, 0.8, 1);
	pCPS = pCPS + 1;

	tgt = pCPS%CIRCLE_PARTICLESYS_NUM;
	circleParticleSyses[tgt].init(2, white, Vector3f(pos.x, pos.y, pos.z), pvn * 0.2, pvn * 0.4, 0, 1, 1, 2, 1.2, 1.4);
	circleParticleSyses[tgt].initCircle(5, 6, 1.8, 2, PI / 100, PI / 200, 0.8, 1);
	pCPS = pCPS + 1;

	//pCPS = pCPS == CIRCLE_PARTICLESYS_NUM ? 0 : pCPS;
}

PxControlBall::PxControlBall(Color4f c, GLfloat r):PxBall(c,r)
{
	
}
