#include"perlinNoise.h"

PerlinData GenerateWhiteNoise(int width, int height) {
	float** noise = new float*[height];
	for (int i = 0;i < height;i++) {
		noise[i] = new float[width];
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			noise[i][j] = rand()%10000/10000.0;
		}
	}
	PerlinData result;
	result.height = height;
	result.width = width;
	result.data = noise;
	return result;
}

float Interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * .5;

	return  a*(1 - f) + b*f;
}

PerlinData GenerateSmoothNoise(PerlinData baseNoise, int octave)
{
	int width = baseNoise.width;
	int height = baseNoise.height;

	float** smoothNoise = new float*[height];
	for (int i = 0;i < height;i++) {
		smoothNoise[i] = new float[width];
	}

	int samplePeriod = 1 << octave; // calculates 2 ^ k  
	float sampleFrequency = 1.0f / samplePeriod;

	for (int i = 0; i < height; i++)
	{
		//calculate the horizontal sampling indices  
		int sample_i0 = (i / samplePeriod) * samplePeriod;
		int sample_i1 = (sample_i0 + samplePeriod) % height; //wrap around  
		float horizontal_blend = (i - sample_i0) * sampleFrequency;

		for (int j = 0; j < width; j++)
		{
			//calculate the vertical sampling indices  
			int sample_j0 = (j / samplePeriod) * samplePeriod;
			int sample_j1 = (sample_j0 + samplePeriod) % width; //wrap around  
			float vertical_blend = (j - sample_j0) * sampleFrequency;

			//blend the top two corners  
			float top = Interpolate(baseNoise.data[sample_i0][sample_j0],
				baseNoise.data[sample_i1][sample_j0], horizontal_blend);

			//blend the bottom two corners  
			float bottom = Interpolate(baseNoise.data[sample_i0][sample_j1],
				baseNoise.data[sample_i1][sample_j1], horizontal_blend);

			//final blend  
			smoothNoise[i][j] = Interpolate(top, bottom, vertical_blend);
		}
	}

	PerlinData result;
	result.height = height;
	result.width = width;
	result.data = smoothNoise;
	return result;
}



PerlinData createPerlinData(int width, int height, int octave, float persistance) {
	PerlinData baseNoise = GenerateWhiteNoise(width, height);
	PerlinData *smoothNoise = new PerlinData[octave];


							  //generate smooth noise  
	for (int i = 0; i < octave; i++)
	{
		smoothNoise[i] = GenerateSmoothNoise(baseNoise, i);
	}

	float** perlinNoise = new float*[height];
	for (int i = 0;i < height;i++) {
		perlinNoise[i] = new float[width];
	}
	float amplitude = 1.0f;
	float totalAmplitude = 0.0f;

	//blend noise together  
	for (int oct = octave - 1; oct >= 0; oct--)
	{
		amplitude *= persistance;
		totalAmplitude += amplitude;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				perlinNoise[i][j] += smoothNoise[oct].data[i][j] * amplitude;
			}
		}
	}

	//normalization  
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			perlinNoise[i][j] /= totalAmplitude;
		}
	}
	baseNoise.clear();
	for (int i = 0; i < octave; i++)
	{
		smoothNoise[i].clear();
	}

	PerlinData result;
	result.height = height;
	result.width = width;
	result.data = perlinNoise;
	return result;
}

PerlinImage createPerlinFireImage(int width, int height, int octave, float persistance) {
	PerlinData perlin = createPerlinData(width, height, octave, persistance);
	GLubyte* imagedata = (GLubyte*)malloc(height*width*sizeof(GLubyte) * 3);
	GLubyte color[3];
	for (int i = 0;i < height;i++) {
		for (int j = 0;j < width;j++) {
			color[0] = perlin.data[i][j] * 255;
			color[1] = perlin.data[i][j]* perlin.data[i][j] * 200;
			color[2] = perlin.data[i][j] * perlin.data[i][j] * perlin.data[i][j]*200;
			memcpy((void*)(imagedata + (i*width + j) * 3), color, sizeof(GLubyte) * 3);
		}
	}
	PerlinImage result;
	perlin.clear();
	result.imgType = GL_RGB;
	result.height = height;
	result.width = width;
	result.data = imagedata;
	return result;
}

PerlinImage createPerlinGrayImage(int width, int height, int octave, float persistance) {
	PerlinData perlin = createPerlinData(width, height, octave, persistance);
	GLubyte* imagedata = (GLubyte*)malloc(height*width*sizeof(GLubyte) * 3);
	GLubyte color[3];
	for (int i = 0;i < height;i++) {
		for (int j = 0;j < width;j++) {
			color[0] = perlin.data[i][j] * 255;
			color[1] = perlin.data[i][j] * 255;
			color[2] = perlin.data[i][j] * 255;
			memcpy((void*)(imagedata + (i*width + j) * 3), color, sizeof(GLubyte) * 3);
		}
	}
	PerlinImage result;
	perlin.clear();
	result.imgType = GL_RGB;
	result.height = height;
	result.width = width;
	result.data = imagedata;
	return result;
}

PerlinImage createPerlinGreenImage(int width, int height, int octave, float persistance) {
	PerlinData perlin = createPerlinData(width, height, octave, persistance);
	GLubyte* imagedata = (GLubyte*)malloc(height*width*sizeof(GLubyte) * 3);
	GLubyte color[3];
	for (int i = 0;i < height;i++) {
		for (int j = 0;j < width;j++) {
			color[1] = perlin.data[i][j] * 255;
			color[0] = perlin.data[i][j] * perlin.data[i][j] * 200;
			color[2] = perlin.data[i][j] * perlin.data[i][j] * 200;
			memcpy((void*)(imagedata + (i*width + j) * 3), color, sizeof(GLubyte) * 3);
		}
	}
	PerlinImage result;
	perlin.clear();
	result.imgType = GL_RGB;
	result.height = height;
	result.width = width;
	result.data = imagedata;
	return result;
}

PerlinImage createPerlinLightYelloImage(int width, int height, int octave, float persistance) {
	PerlinData perlin = createPerlinData(width, height, octave, persistance);
	GLubyte* imagedata = (GLubyte*)malloc(height*width*sizeof(GLubyte) * 3);
	GLubyte color[3];
	for (int i = 0;i < height;i++) {
		for (int j = 0;j < width;j++) {
			color[1] = perlin.data[i][j] * 128+128;
			color[0] = perlin.data[i][j] * 128+128;
			color[2] = 0;
			memcpy((void*)(imagedata + (i*width + j) * 3), color, sizeof(GLubyte) * 3);
		}
	}
	PerlinImage result;
	perlin.clear();
	result.imgType = GL_RGB;
	result.height = height;
	result.width = width;
	result.data = imagedata;
	return result;
}

void PerlinImage::clear()
{
	delete[] data;
}

void PerlinData::clear()
{
	for (int i = 0;i < height;i++) {
		delete[] data[i];
	}
	delete[] data;
}
