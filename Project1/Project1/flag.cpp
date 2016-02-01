#include"flag.h"
#include<assert.h>
#include<GL\glut.h>
Flag::Flag(Vector3f l)
{
	location = l;
	texId = 0;
	loadTexture();
	curtime = 0;
}

Flag::~Flag()
{
}

void Flag::draw() {
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	SetMaterialColor(Color4f(1, 1, 1),GL_FRONT_AND_BACK);
	SetMaterialColor(Color4f(1, 1, 1), GL_FRONT_AND_BACK,GL_SPECULAR);
	glPushMatrix();
	{
		glRotatef(180, 0, 1, 1);
		glTranslatef(-location.x, location.y, location.z);
		GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		gluCylinder(quadratic, radius, radius, height, 32, 32);
		gluDeleteQuadric(quadratic);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(location.x, location.y, location.z);
		glEnable(GL_TEXTURE_2D);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		//去掉背面消隐，可以显示双面
		//glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		assert(texId != 0);
		glBindTexture(GL_TEXTURE_2D, texId);
		glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 5, 0, 1, T_NUMPOINTS*3, 4, &cltPoints[0][0][0]);
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &uvPoints[0][0][0]);
		glEnable(GL_MAP2_TEXTURE_COORD_2);
		glEnable(GL_MAP2_VERTEX_3);
		glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
		glEvalMesh2(GL_FILL, 0, 20, 0, 20);
		glDisable(GL_MAP2_VERTEX_3);
		glDisable(GL_MAP2_TEXTURE_COORD_2);
		glDisable(GL_NORMALIZE);
		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void Flag::wave(double time) {
	curtime += 4*time;
	for (int i = 0;i < T_NUMPOINTS-1;i++) {
		for (int j = 0;j < S_NUMPOINTS;j++) {
			cltPoints[j][i][0] = 6*sin(curtime+i+ j/3 )+3*cos(2*(curtime + i + j / 3));
		}
	}
}

bool Flag::loadTexture()
{
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	static char* filename = "resource/flag/flag.bmp";
	if (!LoadTexture(filename, texId)) {
		return false;
	}
	return true;
}
