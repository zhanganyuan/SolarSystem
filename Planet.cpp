#include "Planet.h"

Planet::Planet(float radius, float rotationTime, float orbitTime, GLuint TextureHandle, float distanceFromSun): Star(radius, rotationTime, orbitTime, TextureHandle)
{
	this->distanceFromBase = distanceFromSun;
}


Planet::~Planet()
{

}


void Planet::caculatePosition(float time)
{

	Star::caculatePosition(time);
	//caculate the moons
	for (Moon& moon : moons)
	{
		moon.caculatePosition(time);
	}
}

void Planet::render()
{
	//push matrix
	glPushMatrix();

	Star::render();
	//render the moons
	for (Moon moon : moons)
	{
		moon.render();
	}

	glPopMatrix();
}

void Planet::renderOrbit()
{
	Star::renderOrbit();

	for (Moon moon : moons)
	{
		moon.renderOrbit();
	}
}

void Planet::getPosition(float* vec)
{
	vec[0] = position[0]*distanceFromBase;
	vec[1] = position[1]*distanceFromBase;
	vec[2] = position[2]*distanceFromBase;
}

float Planet::getRadius() const
{
	return radius;
}

void Planet::addMoon(float radius, float rotationTime, float orbitTime, GLuint TextureHandle, float distanceFromPlanet)
{
	moons.push_back(Moon(radius, rotationTime, orbitTime, TextureHandle, distanceFromPlanet));
}



