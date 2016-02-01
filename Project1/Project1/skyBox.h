#pragma once

#include"include\FreeImage.h"
#include"tinyxml\tinyxml.h"
#include"camera.h"
#define TEX_FRONT_IDX 0
#define TEX_BACK_IDX 1
#define TEX_RIGHT_IDX 2
#define TEX_LEFT_IDX 3
#define TEX_TOP_IDX 4
#define TEX_BOTTON_IDX 5

#define VERTEX_PER_FACE 4
struct Skybox {
	GLfloat points[6][VERTEX_PER_FACE][3];
	GLfloat uv[6][VERTEX_PER_FACE][2];
	GLuint texIds[6];
	void loadFromFile(const char* filename);
	void draw(Camera& camera);
};