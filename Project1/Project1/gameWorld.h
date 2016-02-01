#pragma once
#include<vector>
#include<iostream>
#include<gl\glew.h>
#include<GL\glut.h>
#include"ball.h"
#include"pxBall.h"
#include"pxMoveBall.h"
#include"pxFlyBall.h"
#include"pxControlBall.h"
#include"flyBall.h"
#include"camera.h"
#include"skyBox.h"
#include"flag.h"
#include"ground.h"
#include"particle.h"
class GameWorld
{
public:
	/*physx obj*/
	PxFoundation*			gFoundation = NULL;
	PxPhysics*				gPhysics = NULL;
	PxDefaultCpuDispatcher*	gDispatcher = NULL;
	PxScene*				gScene = NULL;
	PxMaterial*				gMaterial = NULL;
	PxCooking*				gCooking = NULL;
	Ground gGround;
	std::vector<Ball> ballVector;
	std::vector<PxBall*> pxBalls;
	std::vector<PxMoveBall*> pxMoveBalls;
	double leftWall;
	double rightWall;
	double topWall;
	double bottomWall;
	Flag* flag;
	Ball* whiteBall;
	FlyBall* flyBall;
	PxFlyBall* pxFlyBall;
	PxControlBall* pxControlBall;
	Camera* camera;
	Object box;
	Skybox skybox;
	void init();
	void addBall(int num);
	void addPxBall(int num);
	void addPxMoveBall(int num);
	void addFlag(Vector3f location);
	void setBox(double left, double top, double right, double bottom);
	void initCPS();
	void loadSkyBox(const char* filename);
	void loadTestObj();
	void simulate(double time);
	void setballState();
	void hitball();
	bool lineCrossBall(Vector3d &position, Vector3d &pointTo, Ball&ball);
	void draw();
	void drawCameraLine();
	int cameraHit();													//判断相机射线是否碰到球
	GameWorld();
	~GameWorld();
	GLuint texid;
private:

};

