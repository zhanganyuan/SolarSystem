#pragma once
#include "Star.h"
#include "Planet.h"
class Sun :
	public Star
{
public:
	std::vector<Planet> planets;
	Sun(float radius, float rotationTime, float TextureHandle);
	~Sun();
	void caculatePosition(float time) override;
	void render() override;
	void renderOrbit() override;
	float getRadius() const;
	void addPlanet(float radius, float rotationTime, float orbitTime, GLuint TextureHandle, float distanceFromSun);
};

