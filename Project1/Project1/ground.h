#pragma once
#include"3DObject.h"
#include<PxPhysicsAPI.h>
#include<PxPhysics.h>
using namespace physx;
class Ground : public Object {
public:
	PxTriangleMesh* pxMesh;
	PxRigidStatic* actor;
	bool cookingMesh(PxPhysics&gPhysx, PxCooking&gCooking);
	PxRigidStatic* createActor(PxMaterial& mtl,PxPhysics&gPhysics);
	Ground();
	~Ground();
};
