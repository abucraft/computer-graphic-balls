#include"gameWorld.h"
#define GROUND_FILE "model/ground/ground.obj"
#define BOX_FILE "model/box/box.obj"
#define CIRCLE_PARTICLE_TEX_FILE "resource/particle/circle.png"
#define TEST_TEXTURE "model/ground/ground.png"
static Object testObj;
static PxDefaultAllocator gAllocator;
static PxDefaultErrorCallback gErrorCallback;
void GameWorld::init()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation) {
		printf("PxCreateFoundation failed!");
	}
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);
	if (!PxInitExtensions(*gPhysics)) {
		printf("init error pxinit\n");
	}
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(gPhysics->getTolerancesScale()));
	if (!gCooking) {
		printf("PxCreateCooking failed!\n");
	}
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -98.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 0, 1, 400), *gMaterial);
	gScene->addActor(*groundPlane);
	groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 0, -1, 400), *gMaterial);
	gScene->addActor(*groundPlane);
	groundPlane = PxCreatePlane(*gPhysics, PxPlane(1, 0, 0, 280), *gMaterial);
	gScene->addActor(*groundPlane);
	groundPlane = PxCreatePlane(*gPhysics, PxPlane(-1, 0, 0, 280), *gMaterial);
	gScene->addActor(*groundPlane);
	groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, -1, 0, 600), *gMaterial);
	gScene->addActor(*groundPlane);
	groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, -1), *gMaterial);
	gScene->addActor(*groundPlane);
	gGround.loadFromObj(GROUND_FILE);
	gGround.cookingMesh(*gPhysics, *gCooking);
	gGround.createActor(*gMaterial, *gPhysics);
	gScene->addActor(*(gGround.actor));

	//包围盒
	box.loadFromObj(BOX_FILE);

	//初始化飞球
	pxFlyBall = new PxFlyBall(Color4f(1.0, 200 / 255.0, 0), 5.0);
	Material mtl;
	PerlinImage perlinYellow = createPerlinLightYelloImage(40, 40, 6, 1.8);
	PerlinTexture(perlinYellow, mtl.kd_texid);
	mtl.ka = Color4f(1, 1, 1, 1);
	mtl.kd = Color4f(1, 1, 1, 1);
	mtl.ks = Color4f(1, 1, 1, 1);
	pxFlyBall->mtl = mtl;
	pxFlyBall->createPxBall(*gPhysics, PxTransform(PxVec3(rand() % 500 - 250, 100, rand() % 700 - 350)), *gMaterial);
	pxFlyBall->pxActor->setAngularDamping(0.5);
	perlinYellow.clear();
	gScene->addActor(*(pxFlyBall->pxActor));

	//初始化白球
	pxControlBall = new PxControlBall(Color4f(1.0, 1.0, 1.0), 5.0);
	PerlinImage perlinGray = createPerlinGrayImage(40, 40, 6, 1.8);
	PerlinTexture(perlinGray, mtl.kd_texid);
	mtl.ka = Color4f(1, 1, 1, 1);
	mtl.kd = Color4f(1, 1, 1, 1);
	mtl.ks = Color4f(0, 0, 0, 0);
	pxControlBall->mtl = mtl;
	pxControlBall->createPxBall(*gPhysics, PxTransform(PxVec3(0, 50, 0)), *gMaterial);
	pxControlBall->pxActor->setAngularDamping(0.5);
	perlinGray.clear();
	gScene->addActor(*(pxControlBall->pxActor));
	
	GLfloat light1PosType[] = { -5.0,1.0,5.0,0.0 };
	GLfloat whiteColor[] = { 1.0,1.0,1.0,1.0 };
	GLfloat darkColor[] = { 0.4,0.4,0.4,1 };
	GLfloat specColor[] = { 1,1,1,1 };
	GLfloat lightColor[] = { 1,1,1,1 };
	GLfloat globalAmbient[] = { 0.2,0.2,0.2,1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	glLightfv(GL_LIGHT1, GL_AMBIENT, darkColor);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specColor);
	glLightfv(GL_LIGHT1, GL_POSITION, light1PosType);
}

void GameWorld::addBall(int num)
{
	for (int i = 0;i < num;i++) {
		int type = i % 2 == 0 ? POSITIVE_BALL : NEGATIVE_BALL;
		//type = POSITIVE_BALL;
		Color4f color;
		switch (type)
		{
		case POSITIVE_BALL:
			color.red = 50/255.0;
			color.green = 225/255.0;
			color.blue = 50/255.0;
			break;
		case NEGATIVE_BALL:
			color.red = 100/255.0;
			color.green = 0;
			color.blue = 255/255.0;
			break;
		default:
			break;
		}
		double friction = 0.5;
		Vector3d direction;
		direction.x = rand() % 100;
		direction.z = rand() % 100;
		direction.y = 0.0;
		Vector3d position;
		position.x = rand() % 100;
		position.y = 5.0;
		position.z = rand() % 100;
		for (int j = 0;j < ballVector.size();j++) {
			Vector3d ballPos = ballVector[j].position;
			Vector3d deltpos = position - ballPos;
			if (deltpos.length() <= 10.0) {
				position.x = rand() % 100;
				position.y = 5.0;
				position.z = rand() % 100;
				break;
			}
		}
		Ball newball = Ball(5.0, friction, 60, direction,position, color, type);
		ballVector.push_back(newball);
	}
}

void GameWorld::addPxBall(int num)
{
	Color4f color(100 / 255.0,0, 255 / 255.0);
	for (int i = 0;i < num;i++) {
		PxBall* newball =new PxBall(color, 5.0);
		Material mtl;
		PerlinImage perlinFire = createPerlinFireImage(40, 40,6,1.414);
		PerlinTexture(perlinFire, mtl.kd_texid);
		mtl.ka = Color4f(1,1,1,1);
		mtl.kd = Color4f(1, 1, 1, 1);
		newball->mtl = mtl;
		newball->createPxBall(*gPhysics, PxTransform(PxVec3(rand() % 500 - 250, 100, rand() % 700 - 350)), *gMaterial);
		newball->pxActor->setAngularDamping(0.5);
		gScene->addActor(*(newball->pxActor));
		pxBalls.push_back(newball);
		perlinFire.clear();
	}
}

void GameWorld::addPxMoveBall(int num)
{
	Color4f color(50 / 255.0, 225 / 255.0, 50 / 255.0);
	for (int i = 0;i < num;i++) {
		PxMoveBall* newball = new PxMoveBall(color, 5.0, 100.0f);
		Material mtl;
		PerlinImage perlinGreen = createPerlinGreenImage(40, 40, 6, 1.8);
		PerlinTexture(perlinGreen, mtl.kd_texid);
		mtl.ka = Color4f(1, 1, 1, 1);
		mtl.kd = Color4f(1, 1, 1, 1);
		newball->mtl = mtl;
		newball->createPxBall(*gPhysics, PxTransform(PxVec3(rand() % 500 - 250, 100, rand() % 700 - 350)), *gMaterial);
		newball->pxActor->setAngularDamping(0.5);
		gScene->addActor(*(newball->pxActor));
		pxMoveBalls.push_back(newball);
		perlinGreen.clear();
	}
}

void GameWorld::addFlag(Vector3f location)
{
	flag = new Flag(location);
}

void GameWorld::setBox(double left, double top, double right, double bottom)
{
	leftWall = left;
	topWall = top;
	rightWall = right;
	bottomWall = bottom;
}

void GameWorld::initCPS()
{
	GLuint texId = 0;
	if (!LoadTexture(CIRCLE_PARTICLE_TEX_FILE, texId, GL_BGRA, GL_RGBA)) {
		printf("load texture fail:\s\n", CIRCLE_PARTICLE_TEX_FILE);
	}
	initCPSS(camera, texId);
	printf("init particle system success\n");
}


void GameWorld::loadSkyBox(const char * filename)
{
	skybox.loadFromFile(filename);
}

void GameWorld::loadTestObj()
{
	testObj.loadFromObj(GROUND_FILE);
}

void GameWorld::simulate(double time)
{
	//以前的二维碰撞
	/*setballState();
	for (int i = 0;i < ballVector.size();i++) {
		for (int j = i+1;j < ballVector.size();j++) {
			ballVector[i].crashBall(ballVector[j],time);
		}
	}
	for (int i = 0;i < ballVector.size();i++) {
		if (ballVector[i].type == NEGATIVE_BALL) {
			whiteBall->crashBall(ballVector[i], time);
		}
		flyBall->crashBall(ballVector[i], time);
		ballVector[i].crashBox(leftWall, topWall, rightWall, bottomWall,time);
		ballVector[i].move(time);
	}
	flyBall->crashBall(*whiteBall, time);
	whiteBall->crashBox(leftWall, topWall, rightWall, bottomWall, time);
	whiteBall->move(time);
	flyBall->crashBox(leftWall, topWall, rightWall, bottomWall, time);
	flyBall->move(time);
	*/

	//physx模拟
	gScene->fetchResults();
	//printf("get result\n");
	for (int i = 0;i < pxMoveBalls.size();i++) {
		pxMoveBalls[i]->move();
	}
	if (pxFlyBall != NULL) {
		pxFlyBall->move();
	}
	
	gScene->simulate(1.0f / 60.0f);
	simulateCPSS(1.0f / 60.0f);
	if (flag != NULL) {
		flag->wave(time);
	}
}

void GameWorld::setballState()
{
	Vector3d pointTo = camera->center - camera->position;
	pointTo.normalize();
	Vector3d start = camera->position + camera->up*(camera->ywmin);
	for (int i = 0;i < ballVector.size();i++) {
		if (lineCrossBall(start, pointTo, ballVector[i])) {
			ballVector[i].isChosed = true;
		}
		else
		{
			ballVector[i].isChosed = false;
		}
	}
	if (lineCrossBall(start, pointTo, *whiteBall)) {
		whiteBall->isChosed = true;
	}
	else
	{
		whiteBall->isChosed = false;
	}
	if (lineCrossBall(start, pointTo, *flyBall)) {
		flyBall->isChosed = true;
	}
	else
	{
		flyBall->isChosed = false;
	}
}

void GameWorld::hitball()
{
	Vector3d pointTo = camera->center - camera->position;
	pointTo.normalize();
	Vector3d start = camera->position + camera->up*(camera->ywmin);
	if (lineCrossBall(start, pointTo, *whiteBall)) {
		pointTo = (*whiteBall).position - start;
		pointTo.y = 0.0;
		pointTo.normalize();
		pointTo = pointTo * 60;
		whiteBall->direction = whiteBall->direction*whiteBall->speed + pointTo;
		whiteBall->speed = whiteBall->direction.length();
		whiteBall->direction.normalize();
	}
}

bool GameWorld::lineCrossBall(Vector3d & position, Vector3d & pointTo, Ball & ball)
{
	Vector3d width,line1,line2;
	line1 = ball.position - position;
	double length = line1*pointTo;
	line2 = pointTo*length;
	width = ball.position - (position + line2);
	if (width.length() < ball.radius) {
		return true;
	}
	return false;
}

void GameWorld::draw()
{
	//在这里也要设置一下光源位置，不然的话光源会随着摄像机一起动
	GLfloat light1PosType[] = { -5.0,1.0,5.0,0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1PosType);
	skybox.draw(*camera);
	glEnable(GL_LIGHTING);
	

	//glEnable(GL_POLYGON_SMOOTH);

	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	//setballState();

	glEnable(GL_LIGHT1);
	if (pxControlBall != NULL) {
		PxTransform trans = pxControlBall->pxActor->getGlobalPose();
		PxVec3 pos = trans.p;
		pos.y += 200;
		GLfloat direction[] = { 0,-1,0 };
		GLfloat ambient2[] = { 0.3, 0.3, 0.3, 1 };
		GLfloat diffuse2[] = { 1,1,1,1 };
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.002);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.00001);
		glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 80.0f);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 100.0f);
		glLightfv(GL_LIGHT2, GL_POSITION, (GLfloat*)&pos); 
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction);
		
		glEnable(GL_LIGHT2);
		pxControlBall->draw();
	}
	
	for (int i = 0;i < ballVector.size();i++) {
		ballVector[i].draw();
	}
	for (int i = 0;i < pxBalls.size();i++) {
		pxBalls[i]->drawWithMtl();
	}
	for (int i = 0;i < pxMoveBalls.size();i++) {
		pxMoveBalls[i]->drawWithMtl();
	}
	if (pxFlyBall != NULL) {
		pxFlyBall->drawWithMtl();
	}
	
	//drawCameraLine();
	testObj.draw();
	glColor3f(1, 1, 1);
	if (flag != NULL) {
		flag->draw();
	}
	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	box.draw();
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	gGround.draw();
	drawCPSS();
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		std::cerr << "GL_" << error.c_str() << std::endl;
		err = glGetError();
	}
}

void GameWorld::drawCameraLine()
{
	static double delt = 0;
	static int divide = 10;
	if (camera == NULL) {
		return;
	}
	Vector3d pointTo = camera->center - camera->position;
	pointTo.normalize();
	Vector3d start, end,horizental;
	start = camera->position + camera->up*(camera->ywmin);
	end = start + pointTo*(camera->zfar/2);
	horizental = cross(pointTo, camera->up);
	horizental.normalize();
	double length = (end - start).length();
	double perlength = length / divide;
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_QUADS);
	start = start + pointTo*delt;
	Vector3d left, right, top, bottom;
	for (int i = 0;i < divide;i++) {
		left = start - horizental*(perlength / 100);
		right = start + horizental*(perlength / 100);
		top = right + pointTo*(perlength / 2);
		bottom = left+ pointTo*(perlength / 2);
		glVertex3d(left.x, left.y, left.z);
		glVertex3d(right.x, right.y, right.z);
		glVertex3d(top.x, top.y, top.z);
		glVertex3d(bottom.x, bottom.y, bottom.z);
		start = start + pointTo*perlength;
	}
	glEnd();
	if (delt >= perlength) {
		delt = 0;
	}
	else
	{
		delt += 0.5;
	}
}

int GameWorld::cameraHit()
{
	return 0;
}

GameWorld::GameWorld()
{
	whiteBall = 0;
	flyBall = 0;
	flag = NULL;
	pxFlyBall = NULL;
	pxControlBall = NULL;
}

GameWorld::~GameWorld()
{
	if(whiteBall!=0)
		delete whiteBall;
	if(flyBall!=0)
		delete flyBall;
	if (flag != NULL)
		delete flag;
	if (pxFlyBall != NULL)
		delete pxFlyBall;
	if (pxControlBall != NULL)
		delete pxControlBall;
	for (int i = 0;i < pxBalls.size();i++) {
		delete pxBalls[i];
	}
	gGround.~Ground();
	gScene->release();
	gDispatcher->release();
	gPhysics->release();
	gFoundation->release();
}