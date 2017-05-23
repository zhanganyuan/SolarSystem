#pragma once
class Camera
{
	float forwardVec[3];// a vector pointing in the direction youre facing
	float rightVec[3];// a vector pointing to the right of where your facing (to describe orientation
	float upVec[3];// a vector pointing upwards from where youre facing
	float position[3];// a vector describing the position of the Camera
	float cameraSpeed;
	float cameraTurnSpeed;// the Camera speed
public:
	Camera();
	~Camera();
	void transformOrientation();// transform the opengl view matrix for the orientation
	void transformTranslation();// transform the opoengl view matrix for the translation
	void pointAt(float* targetVec);// points the Camera at the given point in 3d space
	void speedUp();// speed up the Camera speed
	void slowDown();// slow down the Camera speed
	void forward();// move the Camera forward
	void left();// strafe left
	void right();// strafe right
	void backward();// move the Camera backward
	void rollRight();// roll the Camera to the right
	void rollLeft();// roll the Camera to the left
	void pitchUp();// pitch the Camera up
	void pitchDown();// pitch the Camera down
	void yawLeft();// yaw left
	void yawRight();// yaw right
};
