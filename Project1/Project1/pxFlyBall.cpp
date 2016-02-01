#include "pxFlyBall.h"

void PxFlyBall::move()
{
	time++;
	
	if (time % 3000 < 1600) {
		PxVec3 v = pxActor->getLinearVelocity();
		if (fabs(v.x)< 0.0001 && fabs(v.z)<0.0001) {
			GLfloat angle = (rand() % 360) / 360.0*PxPi;
			pxActor->addForce(PxVec3(PxSin(angle) * 50, 0, PxCos(angle) * 50), PxForceMode::eACCELERATION);
		}
		else {
			if (fabs(v.x) < 50 && fabs(v.z) < 50) {
				PxVec3 nmvec = v.getNormalized();
				pxActor->addForce(PxVec3(nmvec.x*50.0, 0, nmvec.z*50.0), PxForceMode::eACCELERATION);
			}
		}
		PxTransform t = pxActor->getGlobalPose();
		if(t.p.y<300)
			pxActor->addForce(PxVec3(0, rand()%200, 0.0), PxForceMode::eACCELERATION);
		if (particleTime == 0) {
			addCPSS();
		}
	}
	//particleTime++;
	//particleTime = particleTime > 5 ? 0 : particleTime;
	time = time > 3000 ? 0 : time;
}

void PxFlyBall::addCPSS()
{
	PxVec3 v = pxActor->getLinearVelocity();
	PxTransform trans = pxActor->getGlobalPose();
	PxVec3 pos = trans.p;
	PxVec3 vn = v.getNormalized();
	extern GLuint pCPS;
	extern CircleParticleSys circleParticleSyses[CIRCLE_PARTICLESYS_NUM];
	Color4f white(1.0, 1.0, 1.0, 1.0);
	GLuint tgt = pCPS%CIRCLE_PARTICLESYS_NUM;
	Vector3f pvn(-v.x, -v.y, -v.z);
	circleParticleSyses[tgt].init(1.2,white, Vector3f(pos.x, pos.y, pos.z), pvn * 0.2, pvn * 0.4, 0, 3, 1, 2, 1.2, 1.4);
	circleParticleSyses[tgt].initCircle(2,4, 1.8, 2, PI , PI / 2, 1.2, 1.2);
	pCPS = pCPS + 1;
	//pCPS = pCPS == CIRCLE_PARTICLESYS_NUM ? 0 : pCPS;
}

PxFlyBall::PxFlyBall(Color4f c, GLfloat r):PxBall(c,r)
{
	particleTime = 0;
	time = 0;
}
