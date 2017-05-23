#pragma once
#include <GL/glut.h>

class Star
{
	/*radius, rotationTime, orbitTime, position, TextureHandle*/
protected:
	float radius;
	float rotationTime;
	float orbitTime;

	float rotation;
	float distanceFromBase;


public:
	float position[3] = {0,0,0};
	GLuint TextureHandle;
	Star(float radius, float rotationTime, float orbitTime, GLuint TextureHandle);
	virtual ~Star();
	virtual void caculatePosition(float time);
	virtual void render();
	virtual void renderOrbit();
};
