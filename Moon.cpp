#include <GL/glut.h>
#include "Moon.h"
#include "Globals.h"

Moon::Moon(float radius, float rotationTime, float orbitTime, GLuint TextureHandle, float distanceFromPlanet): Star(radius, rotationTime, orbitTime, TextureHandle)
{
	this->distanceFromBase = distanceFromPlanet;
}


Moon::~Moon()
{
}

void Moon::caculatePosition(float time)
{
	Star::caculatePosition(time);
}

void Moon::render()
{
	//push matrix
	glPushMatrix();
	Star::render();
	glPopMatrix();
}

void Moon::renderOrbit()
{
	glPushMatrix();
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);
	Star::renderOrbit();
	glPopMatrix();
}
