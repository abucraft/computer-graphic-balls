#include "particle.h"

void Particle::move(GLfloat time)
{
	if (!dead) {
		location = location + velocity*time;
		velocity = velocity + (velocity*accelerate - velocity)*time;
		lifetime = lifetime - decline*time;
		if (lifetime <= 0.0f) {
			dead = true;
		}
	}
}

Particle::Particle()
{
	dead = false;
}

void CircleParticle::move(GLfloat time)
{
	Particle::move(time);
	if (!dead) {
		angle += angleSpeed*time;
		angleSpeed += (angleSpeed*angleDump-angleSpeed)*time;
		radius += (radius*radiusChange-radius)*time;
	}
}

Vector3f CircleParticle::getLocation()
{
	GLfloat zxLength = sqrtf(velocity.x*velocity.x + velocity.z*velocity.z);
	Vector3d pointTo;
	if (fabsf(zxLength) < MIN_FLOAT) {
		pointTo.z = 0;
		pointTo.y = 0;
		pointTo.x = 1;
	}
	else {
		GLfloat ylength = velocity.y;
		if (fabsf(ylength) < MIN_FLOAT) {
			pointTo.y = 1;
			pointTo.x = 0;
			pointTo.z = 0;
		}
		else {
			pointTo.y = zxLength;
			pointTo.z = -velocity.z*(ylength / zxLength);
			pointTo.x = -velocity.x*(ylength / zxLength);
		}
	}
	pointTo.normalize();
	GLfloat tmpangle = angle;
	Vector3d result = pointTo;
	/*while (tmpangle > PI/36) {
		result = rotate(Vector3d(velocity.x, velocity.y, velocity.z), Vector3d(0, 0, 0), PI / 36, result);
		tmpangle -= PI / 36;
	}*/
	result = rotate(Vector3d(velocity.x, velocity.y, velocity.z), Vector3d(0, 0, 0), tmpangle, result);
	result.normalize();
	return Vector3f(location.x+result.x*radius,location.y+result.y*radius,location.z+result.z*radius);
}

void CircleParticleSys::init(GLfloat size,Color4f color,Vector3f location, Vector3f minVelocity, Vector3f maxVelocity,GLfloat diverge, GLfloat lifetime, GLfloat minDecline, GLfloat maxDecline, GLfloat minAccelerate, GLfloat maxAccelerate)
{
	this->dead = false;
	this->color = color;
	for (int i = 0;i < CIRCLE_PARTICLE_NUM;i++) {
		particles[i].dead = false;
		particles[i].size = size;
		particles[i].location = location;
		particles[i].velocity = minVelocity+(maxVelocity-minVelocity)*(abs(rand()) % 1000 / 1000.0);
		//particles[i].velocity = Vector3f(1, 1, 1);
		particles[i].maxlife = lifetime;
		particles[i].lifetime = lifetime;
		particles[i].decline = minDecline + (maxDecline-minDecline)*(abs(rand()) % 1000 / 1000.0);
		particles[i].accelerate = minAccelerate + (maxAccelerate-minAccelerate)*(abs(rand()) % 1000 / 1000.0);
	}
}

void CircleParticleSys::initCircle(GLfloat minRadius, GLfloat maxRadius, GLfloat minRadiusChange, GLfloat maxRadiusChange, GLfloat minAngleSpeed, GLfloat maxAngleSpeed, GLfloat minAngleDump, GLfloat maxAngleDump)
{
	for (int i = 0;i < CIRCLE_PARTICLE_NUM;i++) {
		particles[i].radius = minRadius + (maxRadius - minRadius)*(abs(rand()) % 1000 / 1000.0);
		particles[i].radiusChange = minRadiusChange + (maxRadiusChange - minRadiusChange)*(abs(rand()) % 1000 / 1000.0);
		particles[i].angle = PI*2  *(abs(rand()) % 1000 / 1000.0);
		particles[i].angleDump = minAngleDump + (maxAngleDump - minAngleDump)*(abs(rand()) % 1000 / 1000.0);
		particles[i].angleSpeed = minAngleSpeed + (maxAngleSpeed - minAngleSpeed)*(abs(rand()) % 1000 / 1000.0);
	}
}

void CircleParticleSys::simulate(GLfloat time)
{
	if (dead) {
		return;
	}
	for (int i = 0;i < CIRCLE_PARTICLE_NUM;i++) {
		particles[i].move(time);
	}
	isDead();
}

void CircleParticleSys::draw()
{
	if (dead) {
		return;
	}
	if (camera != NULL) {
		Vector3d up = camera->up;
		Vector3d to = camera->center - camera->position;
		Vector3d right = cross(to, up);
		up.normalize();
		right.normalize();
		glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);									// Enable Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, texId);
		int i = 0;
		for (int i = 0;i < CIRCLE_PARTICLE_NUM;i++) {
			if (!particles[i].dead) {
				glPushMatrix();
				Vector3f plct = particles[i].getLocation();
				glTranslatef(plct.x, plct.y, plct.z);
				//glColor4f(1.0, 1.0, 1.0, 1.0);
				glColor4f(color.red, color.green, color.blue, particles[i].lifetime / particles[i].maxlife);
				Vector3d sup = up*particles[i].size;
				Vector3d sright = right*particles[i].size;
				Vector3d tr = sup + sright;
				Vector3d tl = sup - sright;
				Vector3d br = sright - sup;
				Vector3d bl = -sup - sright;
				glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(1.0, 1.0);
				glVertex3d(tr.x, tr.y, tr.z);
				glTexCoord2f(0.0, 1.0);
				glVertex3d(tl.x, tl.y, tl.z);
				glTexCoord2f(1.0, 0.0);
				glVertex3d(br.x, br.y, br.z);
				glTexCoord2f(0.0, 0.0);
				glVertex3d(bl.x, bl.y, bl.z);
				glEnd();
				glPopMatrix();
			}
		}
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}
}

bool CircleParticleSys::isDead()
{
	if (dead) {
		return dead;
	}
	for (int i = 0;i < CIRCLE_PARTICLE_NUM;i++) {
		if (!particles[i].dead) {
			return false;
		}
	}
	dead = true;
	return true;
}

CircleParticleSys::CircleParticleSys()
{
	camera = NULL;
	dead = true;
}


GLuint pCPS = 0;
CircleParticleSys circleParticleSyses[CIRCLE_PARTICLESYS_NUM];

void initCPSS(Camera*camera,GLuint texID) {
	for (int i = 0;i < CIRCLE_PARTICLESYS_NUM;i++) {
		circleParticleSyses[i].camera = camera;
		circleParticleSyses[i].texId = texID;
	}
}

void drawCPSS() {
	for (int i = 0;i < CIRCLE_PARTICLESYS_NUM;i++) {
		circleParticleSyses[i].draw();
	}
}
void simulateCPSS(GLfloat time) {
	for (int i = 0;i < CIRCLE_PARTICLESYS_NUM;i++) {
		circleParticleSyses[i].simulate(time);
	}
}

