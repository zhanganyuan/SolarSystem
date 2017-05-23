#include "Sun.h"
#include "Globals.h"


Sun::Sun(float radius, float rotationTime, float TextureHandle): Star(radius, rotationTime, 0, TextureHandle)
{
}


Sun::~Sun()
{
}

void Sun::caculatePosition(float time)
{
	//rotation
	rotation = time * 360 / rotationTime;

	this->position[0] = 0;
	this->position[1] = 0;
	this->position[2] = 0;

	//caculate planets
	for (Planet& planet : planets)
	{
		planet.caculatePosition(time);
	}
}

void Sun::render()
{
	//push matrix
	glPushMatrix();

	glRotatef(rotation, 0.0f, 0.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, TextureHandle);

	GLUquadricObj* q_obj = gluNewQuadric();
	gluQuadricTexture(q_obj, GLU_TRUE);
	gluQuadricNormals(q_obj, GLU_SMOOTH);
	float radiusScaled = 0.5f;

	glDisable(GL_LIGHTING);

	if (renderMode == "None")
	{
		gluSphere(q_obj, radiusScaled, 30, 30);
	}
	else if (renderMode == "Wire")
	{
		glutWireSphere(radiusScaled, 30, 30);
	}
	else if (renderMode == "Solid")
	{
		glutSolidSphere(radiusScaled, 30, 30);
	}

	glEnable(GL_LIGHTING);
	for (int i = 0; i < planets.size(); ++i)
	{
		planets[i].render();
	}
	glPopMatrix();
}

void Sun::renderOrbit()
{
	for (Planet planet : planets)
	{
		planet.renderOrbit();
	}
}

float Sun::getRadius() const
{
	return radius;
}

void Sun::addPlanet(float radius, float rotationTime, float orbitTime, GLuint TextureHandle, float distanceFromSun)
{
	planets.push_back(Planet(radius, rotationTime, orbitTime, TextureHandle, distanceFromSun));
}

