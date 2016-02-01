#include<gl/glew.h>
#include<GL\glut.h>
#include"myWindow.h"
#include<time.h>
#include"camera.h"
#include"gameWorld.h"
#include<stdio.h>
#include"3DObject.h"
//#include"physx_test.h"
#define RENDER_CLOCK 1
#define SIMULATE_CLOCK 2
#define SKYBOX_RESOURCE "resource/skybox2/skybox.xml"

Vector2d mousePoint;
Camera mainCamera;
MyWindow mainWindow;
GameWorld gameWorld;
TITLEBARINFO mainWindowTitle;
bool cursorLock;
bool pause = false;
int curTime = 0;
int lastTime = 0;
char* title = "按esc键解除鼠标锁定";



void centerCursor() {
	mousePoint.x = (mainWindow.rect.left + mainWindow.rect.right) / 2;
	mousePoint.y = (mainWindow.rect.top + mainWindow.rect.bottom) / 2;
	SetCursorPos(mousePoint.x, mousePoint.y);
}

void resizeWindow() {
	GetClientRect(mainWindow.handler, &mainWindow.rect);
	POINT pointlt, pointrb;
	pointlt.x = mainWindow.rect.left;
	pointlt.y = mainWindow.rect.top;
	pointrb.x = mainWindow.rect.right;
	pointrb.y = mainWindow.rect.bottom;
	ClientToScreen(mainWindow.handler, &pointlt);
	ClientToScreen(mainWindow.handler, &pointrb);
	mainWindow.rect.left = pointlt.x;
	mainWindow.rect.top = pointlt.y;
	mainWindow.rect.right = pointrb.x;
	mainWindow.rect.bottom = pointrb.y;
}

void lockCursor() {
	resizeWindow();
	centerCursor();
	cursorLock = true;
	ShowCursor(false);
}

void freeCursor() {
	cursorLock = false;
	ShowCursor(true);
}

void reshapeFcn(GLint newWidth, GLint newHeight) {
	resizeWindow();
	glViewport(0, 0, newWidth, newHeight);
	mainCamera.xwmin = newWidth / (-20);
	mainCamera.xwmax = newWidth / 20;
	mainCamera.ywmin = newHeight / -20;
	mainCamera.ywmax = newHeight / 20;
	mainCamera.setProjection();
}

void keyDown(GLubyte key, int x, int y) {
	switch (key)
	{
	case VK_ESCAPE:
		freeCursor();
		break;
	case 'w':case 'W':
		if (cursorLock) {
			mainCamera.horizentalMove(5.0, 0.0);
		}
		break;
	case 's':case'S':
		if (cursorLock) {
			mainCamera.horizentalMove(-5.0, 0.0);
		}
		break;
	case 'a':case 'A':
		if (cursorLock) {
			mainCamera.horizentalMove(0.0, 5.0);
		}
		break;
	case 'd':case 'D':
		if (cursorLock) {
			mainCamera.horizentalMove(0.0, -5.0);
		}
		break;
	case 'q':case 'Q':
		if (cursorLock) {
			mainCamera.verticalMove(5.0);
		}
		break;
	case 'e':case 'E':
		if (cursorLock) {
			mainCamera.verticalMove(-5.0);
		}
		break;
	case ' ':
		pause = !pause;
		break;
	case 'z':case 'Z':
		if (gameWorld.pxControlBall) {
			gameWorld.pxControlBall->jump();
		}
	default:
		break;
	}
}
void specialKeyDown(int key, int x, int y) {
	int front = 0;
	int right = 0;
	if (key == GLUT_KEY_LEFT) {
		right = -1;
	}
	if (key == GLUT_KEY_RIGHT) {
		right = 1;
	}
	if (key == GLUT_KEY_UP){
		front = 1;
	}
	if (key == GLUT_KEY_DOWN) {
		front = -1;
	}
	if (gameWorld.pxControlBall) {
		gameWorld.pxControlBall->move(front, right, mainCamera);
	}
}

void mouseClick(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			if (!cursorLock) {
				lockCursor();
			}
			else {
				//gameWorld.hitball();
			}
		}
		break;
	default:
		break;
	}
}

void myDisplay(void)
{
	mainCamera.setLookAt();
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	gameWorld.draw();
	//displayPhysics();
	glFlush();
	glutSwapBuffers();
}

void render(int time) {
	myDisplay();
	curTime = glutGet(GLUT_ELAPSED_TIME);
	if (lastTime == 0) {
		lastTime = curTime;
	}
	else
	{
		int delt = curTime - lastTime;
		if (!pause) {
			gameWorld.simulate((double)delt / 1000.0);
		}
		lastTime = curTime;
	}
	glutTimerFunc(16, render, RENDER_CLOCK);
}

void gameLoop(int time) {
	curTime = glutGet(GLUT_ELAPSED_TIME);
	if (lastTime == 0) {
		lastTime = curTime;
	}
	else
	{
		int delt = curTime - lastTime;
		if (!pause) {
			gameWorld.simulate((double)delt / 1000.0);
		}
		lastTime = curTime;
	}
	glutTimerFunc(2, gameLoop, SIMULATE_CLOCK);
}

void mousePassiveMove(int x, int y) {
	if (cursorLock) {
		int cx = x + mainWindow.rect.left;
		int cy = y + mainWindow.rect.top;
		double dx = double(cx - mousePoint.x);
		double dy = double(cy - mousePoint.y);
		centerCursor();
		mainCamera.selfRotate(-dx/3, -dy/3);
	}
}



void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	mainCamera.setProjection();
	/*glMatrixMode(GL_PROJECTION);
	glFrustum(-40, 40, -30, 30, 100.0, 1000.0);*/
}

void initWindow() {
	mainWindow.title = title;
	mainWindow.rect.left = 50;
	mainWindow.rect.top = 100;
	mainWindow.rect.right = 600+50;
	mainWindow.rect.bottom = 800+100;
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(800, 600);
	mainWindow.id = glutCreateWindow(title);
	mainWindow.handler = FindWindow(NULL, __TEXT(title));
}

void initCamera() {
	mainCamera.angleX = 0;
	mainCamera.angleY = 0;
	mainCamera.position.x = 200.0;
	mainCamera.position.y = 200.0;
	mainCamera.position.z = 0.0;
	mainCamera.center.x = 0.0;
	mainCamera.center.y = 0.0;
	mainCamera.center.z = 0.0;
	mainCamera.up.x = -1;
	mainCamera.up.y = 1;
	mainCamera.up.z = 0.0;
	mainCamera.xwmin = -40.0;
	mainCamera.xwmax = 40.0;
	mainCamera.ywmin = -30.0;
	mainCamera.ywmax = 30.0;
	mainCamera.znear = 100.0;
	mainCamera.zfar = 4000.0;
	mainCamera.up.normalize();
}

void initGameWorld() {
	//gameWorld.setBox(-100, -100, 100, 100);
	gameWorld.init();
	gameWorld.addFlag(Vector3f(-100, 0, 0));
	//gameWorld.addBall(12);
	gameWorld.addPxBall(6);
	gameWorld.addPxMoveBall(6);
	gameWorld.camera = &mainCamera;
	gameWorld.loadSkyBox(SKYBOX_RESOURCE);
	gameWorld.initCPS();
	//initPhysics(false);
}



int main(int argc, char * argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	initWindow();
	initCamera();
	initGameWorld();
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshapeFcn);
	glutKeyboardFunc(keyDown);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mousePassiveMove);
	glutSpecialFunc(specialKeyDown);
	glutTimerFunc(16, render, RENDER_CLOCK);
	//glutTimerFunc(2, gameLoop, SIMULATE_CLOCK);
	glutMainLoop();
	return 0;
}