#pragma once
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <glm\gtx\transform.hpp>


class Camera
{
public:
	Camera();
	glm::mat4 worldView; 

	glm::vec3 camRot = { 0,0,0 };
	glm::vec3 camLoc = { 0,0,2 };
	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	float zoom = 1.f;
	int width = 800;
	int height = 600;
	float fovy = 3.14159f *.4f / zoom;
	float aspect = (float)width / (float)height;
	float zNear = .01f;
	float zFar = 1000.f;

	glm::mat4 perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);
	glm::vec3 eye = camLoc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);
	glm::mat4 lookAtMat = glm::lookAt(eye, center, up);

	void start(); 
	void upload();
	void control();

	~Camera();
};

