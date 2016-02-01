#pragma once
#include<GL\glew.h>
#include"matrix.h"
#include<list>
#include<vector>
#include<string>
#include<sstream>
#include<map>
#include"textureLoader.h"
#include<fstream>
#include<iostream>
struct Material {
	Color4f kd;
	Color4f ka;
	Color4f ks;
	Color4f ke;
	GLuint illum;
	GLfloat ns;
	GLuint kd_texid;
	GLuint ka_texid;
};

class SubMesh {
public:
	Point *ptBuffer;
	UVPoint *uvBuffer;
	NMPoint *nmBuffer;
	GLuint *ptIndices;
	GLuint vNum;
	GLuint fcNum;
	std::string fcName;
	std::string mtName;
	void draw();
	SubMesh();
	~SubMesh();
};


class Object {
public:
	std::list<SubMesh*> children;
	Point *ptBuffer;
	UVPoint *uvBuffer;
	NMPoint *nmBuffer;
	GLuint vNum;
	GLuint vtNum;
	GLuint vnNum;
	std::map<std::string, Material> material_map;
	Vector3d position;
	void draw();
	void parsePolygon(std::ifstream&ifs,std::string &name);
	int countFaceNumber(std::ifstream &ifs);
	void countNumber(std::ifstream& ifs);
	void loadMtl(const char*filename);
	void loadFromObj(const char*filename);
	Object();
	~Object();
};