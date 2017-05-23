#pragma once
#include "star.h"
class Moon :
	public Star
{

public:
	Moon(float radius, float rotationTime, float orbitTime, GLuint TextureHandle, float distanceFromPlanet);
	virtual ~Moon();
	void caculatePosition(float time) override;
	void render() override;
	void renderOrbit() override;
};

