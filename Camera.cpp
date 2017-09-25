#include "Camera.h"
#include "Engine.h"


Camera::Camera()
{
}

void Camera::start()
{
	worldView = glm::mat4();
}

void Camera::upload()
{

	rotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);
	eye = camLoc;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);
	lookAtMat = glm::lookAt(eye, center, up);

	worldView = perspectiveMat * lookAtMat;

	glUniformMatrix4fv(1, 1, GL_FALSE, &worldView[0][0]);
	glUniform3fv(4, 1, &camLoc.x);
	
}

void Camera::control()
{	
	float sens = .005; 
	int width = 800; 
	int height = 600; 
	double x, y; 
	glfwGetCursorPos(Engine::GLFWwindowPtr, &x, &y);
	glfwGetWindowSize(Engine::GLFWwindowPtr, &width, &height);

	camRot.y -= sens * (x - width *.5f);
	camRot.x -= sens * (y - height *.5f);

	camRot.x = glm::clamp(camRot.x, -.5f * 3.14f, .5f * 3.14f); 

	glfwSetCursorPos(Engine::GLFWwindowPtr, width *.5f, height *.5f);

	// wsad 
	glm::vec3 velocity;
	glm::mat3 R = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	if (glfwGetKey(Engine::GLFWwindowPtr, GLFW_KEY_A)){
		velocity += R*glm::vec3(-1, 0, 0);
	}
	if (glfwGetKey(Engine::GLFWwindowPtr, GLFW_KEY_D)) {
		velocity += R*glm::vec3(1, 0, 0);
	}
	if (glfwGetKey(Engine::GLFWwindowPtr, GLFW_KEY_W)) {
		velocity += R*glm::vec3(0, 0, -1);
	}
	if (glfwGetKey(Engine::GLFWwindowPtr, GLFW_KEY_S)) {
		velocity += R*glm::vec3(0, 0, 1);
	}

	// up/down
	if (glfwGetKey(Engine::GLFWwindowPtr, GLFW_KEY_SPACE)) {
		velocity += R*glm::vec3(0, 1, 0);
	}
	if (glfwGetKey(Engine::GLFWwindowPtr, GLFW_KEY_LEFT_SHIFT)) {
		velocity += R*glm::vec3(0, -1, 0);
	}



	float speed = 7.5f;
	if (velocity != glm::vec3())
		velocity = glm::normalize(velocity) * speed;
	camLoc += velocity  * Engine::time.dt;



}

Camera::~Camera()
{
}
