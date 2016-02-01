#pragma once

#include<GL\glew.h>
#include"include/FreeImage.h"
#include"perlinNoise.h"
bool LoadTexture(const char* filename,	//where to load the file from
	GLuint &texID,
	//does not have to be generated with glGenTextures
	GLenum image_format = GL_BGR,		//format the image is in
	GLint internal_format = GL_RGB,		//format to store the image in
	GLint level = 0 ,					//mipmapping level
	GLint border = 0);

void PerlinTexture(PerlinImage perlin, GLuint &texID, GLenum image_format = GL_RGB, GLint internal_format = GL_RGB, GLint level = 0,GLint border = 0);