#pragma once
#include<GL\glew.h>
#include<random>
class PerlinImage {
public:
	int width;
	int height;
	GLenum imgType;
	void* data;
	void clear();
};

class PerlinData {
public:
	int width;
	int height;
	GLfloat** data;
	void clear();
};

PerlinData createPerlinData(int width, int height, int octave, float persistance);
PerlinImage createPerlinFireImage(int width,int height,int octave = 6,float persistance = 0.5);
PerlinImage createPerlinGrayImage(int width, int height, int octave = 6, float persistance = 0.5);
PerlinImage createPerlinGreenImage(int width, int height, int octave = 6, float persistance = 0.5);
PerlinImage createPerlinLightYelloImage(int width, int height, int octave = 6, float persistance = 0.5);