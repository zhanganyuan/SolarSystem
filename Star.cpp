#include "Star.h"
#include <cmath>
#include "globals.h"


Star::Star(float radius, float rotationTime, float orbitTime, GLuint TextureHandle): radius(radius), rotationTime(rotationTime), orbitTime(orbitTime), rotation(0), distanceFromBase(0), TextureHandle(TextureHandle)
{
}

Star::~Star()
{
}

void Star::caculatePosition(float time)
{
	//angle
	float angle = time * Pi / orbitTime;

	//position (orbit
	this->position[0] = distanceFromBase * cos(angle);
	this->position[1] = distanceFromBase * sin(angle);
	this->position[2] = 0;

	//rotation
	rotation = time * 360 / rotationTime;
}

void Star::render()
{
	//bind textrue
	glBindTexture(GL_TEXTURE_2D, TextureHandle);


	//translate to the right position 
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, position[2] * distanceScale);

	//rotate
	glRotatef(-rotation, 0.0f, 0.0f, 1.0f);

	//render as a GUL sphere quadric object
	GLUquadricObj* q_obj = gluNewQuadric();
	gluQuadricTexture(q_obj, GLU_TRUE);
	gluQuadricNormals(q_obj, GLU_SMOOTH);

	if (renderMode == "None")
	{
		gluSphere(q_obj, radius * starSizeScale, 15, 15);
	}
	else if (renderMode == "Wire")
	{
		glutWireSphere(radius * starSizeScale, 15, 15);
	}
	else if (renderMode == "Solid")
	{
		glutSolidSphere(radius * starSizeScale, 15, 15);
	}
}

void Star::renderOrbit()
{
	// draw a line strip
	glBegin(GL_LINE_STRIP);

	// loop round from 0 to 2*PI and draw around the radius of the orbit using trigonometry
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.1f)
	{
		glVertex3f(sin(angle) * distanceFromBase * distanceScale, cos(angle) * distanceFromBase * distanceScale, 0.0f);
	}
	glVertex3f(0.0f, distanceFromBase * distanceScale, 0.0f);

	glEnd();
}
