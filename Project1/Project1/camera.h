#pragma once
#include"matrix.h"
class Camera {
public:
	Vector3d position;
	Vector3d center;
	Vector3d up;
	double xwmin;
	double xwmax;
	double ywmin;
	double ywmax;
	double znear;
	double zfar;
	double angleX;
	double angleY;
	void selfRotate(double x,double y);
	void horizentalMove(double front, double left);
	void verticalMove(double upm);
	void setLookAt();
	void setProjection();
};