#include "pxMoveBall.h"
void PxMoveBall::move()
{
	PxVec3 v = pxActor->getLinearVelocity();
	if (fabs(v.x - 0.0)<0.0001 && fabs(v.z-0.0)<0.0001) {
		GLfloat angle = (rand() % 360) / 360.0*PxPi;
		pxActor->addForce(PxVec3(PxSin(angle)*50, 0, PxCos(angle)*50),PxForceMode::eACCELERATION);
	}
	else {
		PxReal speed = v.magnitude();
		if (speed <= maxspeed) {
			PxVec3 nmvec = v.getNormalized();
			pxActor->addForce(PxVec3(nmvec.x*50.0, 0, nmvec.z*50.0),PxForceMode::eACCELERATION);
		}
	}
}

PxMoveBall::PxMoveBall(Color4f c, GLfloat r,PxReal maxspeed) :maxspeed(maxspeed),PxBall(c,r){

}