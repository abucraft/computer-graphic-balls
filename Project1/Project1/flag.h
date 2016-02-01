#pragma once
#include"matrix.h"
#include"textureLoader.h"
#define FLAGIMG 200
#define S_NUMPOINTS    4
#define T_NUMPOINTS    5
static const GLfloat height = 100.0;
static const GLfloat radius = 1.0;
class Flag
{
public:
	Flag(Vector3f l);
	~Flag();
	void draw();
	void wave(double time);
	bool loadTexture();
private:
	GLuint texId;
	double curtime;
	Point cltPoints[S_NUMPOINTS][T_NUMPOINTS]= {
		{ { 0., height - 0., radius + 40 },{ 0., height - 0.,radius + 30. },{ 0., height - 0., radius + 20. },{ 0., height - 0., radius + 10. },{ 0., height - 0., radius  } },
		{ { 0., height - 10., radius + 40. },{ 0., height - 10., radius + 30. },{ 0., height - 10., radius + 20. },{ 0., height - 10., radius + 10. },{ 0., height - 10., radius  } },
		{ { 0., height - 20., radius + 40. },{ 0., height - 20., radius + 30. },{ 0., height - 20., radius + 20. },{ 0., height - 20., radius + 10. },{ 0., height - 20., radius  } },
		{ { 0., height - 30., radius + 40. },{ 0., height - 30., radius + 30. },{ 0., height - 30., radius + 20. },{ 0., height - 30., radius + 10. },{ 0., height - 30., radius  } }
	};
	UVPoint uvPoints[2][2] = { {{ 0.0, 1.0 },{ 1.0, 1.0 }},{{ 0.0, 0.0 },{ 1.0, 0.0 }} };
	Vector3f location;
};

