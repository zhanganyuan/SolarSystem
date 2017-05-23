#include "Camera.h"
#include <math.h>
#include <GL/glut.h>
// sets vec to (x,y,z)
void vectorSet(float* vec, float x, float y, float z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

// adds v2 to v1
void vectorAdd(float* v1, float* v2)
{
	v1[0] += v2[0];
	v1[1] += v2[1];
	v1[2] += v2[2];
}

// copies v2 into v1
void vectorCopy(float* v1, float* v2)
{
	v1[0] = v2[0];
	v1[1] = v2[1];
	v1[2] = v2[2];
}

// multiplies vec by the scalar
void vectorMul(float* vec, float scalar)
{
	vec[0] *= scalar;
	vec[1] *= scalar;
	vec[2] *= scalar;
}

// finds the magnitude of a vec using pythag
float lengthOfVec(float* vec)
{
	return sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// normalises a vector to magnitude 1
//变成单位向量
void normaliseVec(float* vec)
{
	vectorMul(vec, 1 / lengthOfVec(vec));
}

// makes a 3x3 rotation matrix from the given angle and axis and pointer to a 3x3 matrix
//矩阵绕axis坐标旋转angle角度
void rotationMatrix(float* matrix, float* axis, float angle)
{
	// using http://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	float cos1 = cos(angle);
	float cos2 = 1 - cos1;
	float sin1 = sin(angle);

	matrix[0] = cos1 + axis[0] * axis[0] * cos2;
	matrix[1] = axis[0] * axis[1] * cos2 - axis[2] * sin1;
	matrix[2] = axis[0] * axis[2] * cos2 + axis[1] * sin1;

	matrix[3] = axis[1] * axis[0] * cos2 + axis[2] * sin1;
	matrix[4] = cos1 + axis[1] * axis[1] * cos2;
	matrix[5] = axis[1] * axis[2] * cos2 - axis[0] * sin1;

	matrix[6] = axis[2] * axis[0] * cos2 - axis[1] * sin1;
	matrix[7] = axis[2] * axis[1] * cos2 + axis[0] * sin1;
	matrix[8] = cos1 + axis[2] * axis[2] * cos2;
}

// multiplies a vector v1 by a matrix and puts the results into vector v2
void mulVecBy(float* v1, float* matrix, float* v2)
{
	v2[0] = v1[0] * matrix[0] + v1[1] * matrix[1] + v1[2] * matrix[2];
	v2[1] = v1[0] * matrix[3] + v1[1] * matrix[4] + v1[2] * matrix[5];
	v2[2] = v1[0] * matrix[6] + v1[1] * matrix[7] + v1[2] * matrix[8];
}

// rotate a vector v1 around the axis v2 by angle and put the result into v3
void rotateAroundVec(float* v1, float* v2, float angle, float* v3)
{
	//. make a rotation matrix for it
	float matrix[16];
	rotationMatrix(matrix, v2, angle);
	// multiply by the matrix
	mulVecBy(v1, matrix, v3);
}


Camera::Camera()
{
	cameraSpeed = 0.005f;
	cameraTurnSpeed = 0.01f;
	vectorSet(position, 0.764331460f, -1.66760659f, 0.642456770f);
	vectorSet(forwardVec, -0.398769796f, 0.763009906f, -0.508720219f);
	vectorSet(rightVec, 0.886262059f, 0.463184059f, 0.000000000f);
	vectorSet(upVec, -0.235630989f, 0.450859368f, 0.860931039f);
}


Camera::~Camera()
{
}

//init visual angle
void Camera::transformOrientation()
{
	gluLookAt(0, 0, 0, forwardVec[0], forwardVec[1], forwardVec[2], upVec[0], upVec[1], upVec[2]);
}

//init visual site
void Camera::transformTranslation()
{
	glTranslatef(-position[0], -position[1], -position[2]);
}

void Camera::pointAt(float* targetVec)
{
	float tempVec[3];
	float up[3] = { 0.0f, 0.0f, 1.0f };

	// first work out the new forward vector by subtracting the target position from the camera position
	forwardVec[0] = targetVec[0] - position[0];
	forwardVec[1] = targetVec[1] - position[1];
	forwardVec[2] = targetVec[2] - position[2];
	// then normalise it to 1 length
	normaliseVec(forwardVec);

	// now to find the right vector we rotate the forward vector -pi/2 around the z axis
	rotateAroundVec(forwardVec, up, -1.57079632679f, tempVec);
	// and remove the y component to make it flat
	tempVec[2] = 0;
	// then normalise it
	normaliseVec(tempVec);
	// and assign it to rightVec
	vectorCopy(rightVec, tempVec);

	// now work out the upvector by rotating the forward vector pi/2 around the rightvector
	rotateAroundVec(forwardVec, rightVec, 1.57079632679f, tempVec);
	vectorCopy(upVec, tempVec);
}
// speed up the camera speed
void Camera::speedUp()
{
	if (cameraSpeed < 1.0f)
		cameraSpeed *= 2;
}

// slow down the camera speed
void Camera::slowDown()
{
	if (cameraSpeed > 0.000001f)
		cameraSpeed /= 2;
}

// move the camera forward
void Camera::forward()
{
	// make a movement vector the right speed facing the forward direction
	float vec[3];
	vectorCopy(vec, forwardVec);
	vectorMul(vec, cameraSpeed);

	// add the movement vec to the position vec
	vectorAdd(position, vec);
}

// move the camera backward
void Camera::backward()
{
	// make a movement vector the right speed facing the backward direction
	float vec[3];
	vectorCopy(vec, forwardVec);
	vectorMul(vec, -cameraSpeed); // -cameraSpeed for backwards

								  // add the movement vec to the position vec
	vectorAdd(position, vec);
}

// strafe left
void Camera::left()
{
	// make a movement vector the right speed facing the left direction
	float vec[3];
	vectorCopy(vec, rightVec);
	vectorMul(vec, -cameraSpeed); // -cameraSpeed for left

								  // add the movement vec to the position vec
	vectorAdd(position, vec);
}

// strafe right
void Camera::right()
{
	// make a movement vector the right speed facing the right direction
	float vec[3];
	vectorCopy(vec, rightVec);
	vectorMul(vec, cameraSpeed);

	// add the movement vec to the position vec
	vectorAdd(position, vec);
}

// roll the camera to the right
void Camera::rollRight()
{
	float tempVec[3];
	// rotate the up and right vectors around the forward vector axis for roll
	rotateAroundVec(upVec, forwardVec, cameraTurnSpeed, tempVec);
	vectorCopy(upVec, tempVec);

	rotateAroundVec(rightVec, forwardVec, cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}

// roll the camera to the left
void Camera::rollLeft()
{
	float tempVec[3];
	// rotate the up and right vectors around the forward vector axis for roll
	rotateAroundVec(upVec, forwardVec, -cameraTurnSpeed, tempVec);
	vectorCopy(upVec, tempVec);

	rotateAroundVec(rightVec, forwardVec, -cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}

// pitch the camera up
void Camera::pitchUp()
{
	float tempVec[3];
	// rotate the forward and up vectors around the right vector axis for pitch
	rotateAroundVec(forwardVec, rightVec, cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);

	rotateAroundVec(upVec, rightVec, cameraTurnSpeed, tempVec);
	vectorCopy(upVec, tempVec);
}

// pitch the camera down
void Camera::pitchDown()
{
	float tempVec[3];
	// rotate the forward and up vectors around the right vector axis for pitch
	rotateAroundVec(forwardVec, rightVec, -cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);

	rotateAroundVec(upVec, rightVec, -cameraTurnSpeed, tempVec);
	vectorCopy(upVec, tempVec);
}

// yaw left
void Camera::yawLeft()
{
	float tempVec[3];
	// rotate the forward and right vectors around the up vector axis for yaw
	rotateAroundVec(forwardVec, upVec, cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);

	rotateAroundVec(rightVec, upVec, cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}

// yaw right
void Camera::yawRight()
{
	float tempVec[3];
	// rotate the forward and right vectors around the up vector axis for yaw
	rotateAroundVec(forwardVec, upVec, -cameraTurnSpeed, tempVec);
	vectorCopy(forwardVec, tempVec);

	rotateAroundVec(rightVec, upVec, -cameraTurnSpeed, tempVec);
	vectorCopy(rightVec, tempVec);
}