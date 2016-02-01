#include "camera.h"

void Camera::selfRotate(double x, double y)
{
	angleX += x;
	if(angleX > 180){
		angleX = angleX - 360;
	}
	if (angleX < -180) {
		angleX = angleX + 360;
	}
	if (angleY + y>90||angleY + y<-90) {
		y = 0;
	}
	angleY += y;
	Vector3d zero;
	zero.x = 0.0;
	zero.y = 0.0;
	zero.z = 0.0;
	Vector3d haxis;
	Vector3d vaxis;
	Vector3d vz = position - center;
	haxis = cross(up, vz);
	haxis.normalize();
	vaxis.x = 0.0;
	vaxis.y = 1.0;
	vaxis.z = 0.0;
	Vector3d mid=rotate(haxis, position, A2R(y), center);
	mid = rotate(vaxis, position, A2R(x), mid);
	center = mid;
	mid = rotate(haxis, zero, A2R(y), up);
	mid = rotate(vaxis, zero, A2R(x), mid);
	up = mid;
}

void Camera::horizentalMove(double front, double left)
{
	Vector3d frontVec = center - position;
	Vector3d leftVec = cross(up, frontVec);
	frontVec.y = 0.0;
	frontVec.normalize();
	leftVec.normalize();
	frontVec = frontVec*front;
	leftVec = leftVec*left;
	position = position + frontVec + leftVec;
	center = center + frontVec + leftVec;
}

void Camera::verticalMove(double upm)
{
	Vector3d upVec;
	upVec.x = 0;
	upVec.y = upm;
	upVec.z = 0;
	position = position + upVec;
	center = center + upVec;
}


void Camera::setLookAt()
{
	Vector3d farpos = position - center;
	farpos.normalize();
	farpos = position + farpos*znear;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(farpos.x, farpos.y, farpos.z,center.x, center.y, center.z, up.x, up.y, up.z);
}

void Camera::setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
}


