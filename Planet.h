#pragma once
#include <vector>
#include "Star.h"
#include "Moon.h"

class Planet :
	public Star
{
	std::vector<Moon> moons;
public:
	Planet(float radius, float rotationTime, float orbitTime, GLuint TextureHandle, float distanceFromSun);
	~Planet();
	void caculatePosition(float time) override;
	void render() override;
	void renderOrbit() override;
	void getPosition(float* vec);
	float getRadius() const;
	void addMoon(float radius, float rotationTime, float orbitTime, GLuint TextureHandle, float distanceFromPlanet);
};

