#include "pxBall.h"

void PxBall::draw()
{
	PxShape* shapes[1];
	pxActor->getShapes(shapes,1);
	const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[0], *pxActor));
	glPushMatrix();
	glMultMatrixf((float*)&shapePose);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(color.red, color.green, color.blue, color.alpha);
	glutSolidSphere(radius, 20, 20);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}

void PxBall::drawWithMtl()
{
	PxShape* shapes[1];
	pxActor->getShapes(shapes, 1);
	const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[0], *pxActor));
	glPushMatrix();
	glMultMatrixf((float*)&shapePose);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetMaterialColor(mtl.ka, GL_FRONT_AND_BACK, GL_AMBIENT);
	SetMaterialColor(mtl.kd, GL_FRONT_AND_BACK, GL_DIFFUSE);
	SetMaterialColor(mtl.ks, GL_FRONT_AND_BACK, GL_SPECULAR);
	glBindTexture(GL_TEXTURE_2D, mtl.kd_texid);
	gluSphere(sphere, radius, 20, 20);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}

void PxBall::setMaterial(Material mtl)
{
	this->mtl = mtl;
}

bool PxBall::createPxBall(PxPhysics & physics, const PxTransform & t, PxMaterial & mtl)
{
	pxActor = PxCreateDynamic(physics, t, PxSphereGeometry(radius), mtl, 10.0f);
	if (!pxActor) {
		return false;
	}
	return true;
}

PxBall::PxBall(Color4f c,GLfloat r):color(c),radius(r)
{
	pxActor = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricTexture(sphere, GL_TRUE);
}

PxBall::~PxBall()
{
	if (pxActor != NULL) {
		pxActor->release();
	}
	gluDeleteQuadric(sphere);
}
