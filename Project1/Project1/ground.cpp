#include "ground.h"

bool Ground::cookingMesh( PxPhysics& gPhysx, PxCooking & gCooking)
{
	PxCookingParams params(gPhysx.getTolerancesScale());
	// disable mesh cleaning - perform mesh validation on development configurations
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	// disable edge precompute, edges are set for each triangle, slows contact generation
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
	// lower hierarchy for internal mesh
	params.meshCookingHint = PxMeshCookingHint::eCOOKING_PERFORMANCE;

	gCooking.setParams(params);
	std::list<SubMesh*>::iterator ppMesh = children.begin();
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = vNum;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = ptBuffer;

	meshDesc.triangles.count = (*ppMesh)->fcNum;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = (*ppMesh)->ptIndices;

#ifdef _DEBUG
	// mesh should be validated before cooked without the mesh cleaning
	bool res = theCooking->validateTriangleMesh(meshDesc);
	PX_ASSERT(res);
#endif

	pxMesh = gCooking.createTriangleMesh(meshDesc,
		gPhysx.getPhysicsInsertionCallback());
	if (!pxMesh) {
		printf("cooking failed\n");
		return false;
	}
	else {
		printf("cooking success\n");
		return true;
	}
}

PxRigidStatic * Ground::createActor(PxMaterial& mtl,PxPhysics& gPhysics)
{
	actor = PxCreateStatic(gPhysics, PxTransform(PxVec3(0, 0, 0)), PxTriangleMeshGeometry(pxMesh), mtl);
	if (actor == NULL) {
		printf("create actor fail\n");
	}
	return actor;
}

Ground::Ground()
{
	pxMesh = NULL;
	actor = NULL;
}

Ground::~Ground()
{
	
	if (actor != NULL) {
		actor->release();
	}
	if (pxMesh != NULL) {
		pxMesh->release();
	}
}
