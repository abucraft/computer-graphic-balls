#include"textureLoader.h"
bool LoadTexture(const char* filename,	//where to load the file from
	GLuint &texID,
	//does not have to be generated with glGenTextures
	GLenum image_format,		//format the image is in
	GLint internal_format,		//format to store the image in
	GLint level ,					//mipmapping level
	GLint border ) {
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;
	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &texID);
	//store the texture ID mapping
	//bind to the new texture ID
	
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// 线形滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 线形滤波
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
																		//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return success
	return true;
}

void PerlinTexture(PerlinImage perlin, GLuint &texID, GLenum image_format, GLint internal_format, GLint level, GLint border) {
	glGenTextures(1, &texID);
	//store the texture ID mapping
	//bind to the new texture ID

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// 线形滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 线形滤波
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, perlin.width, perlin.height,
		border, image_format, GL_UNSIGNED_BYTE, perlin.data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}