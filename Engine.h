#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <iostream>

#include "Timer.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

using glm::vec3; 
using glm::mat4; 
using std::string; 

class Engine
{
public:
	Engine();
	~Engine();
	static Timer time; 

	static void Start();
	static GLFWwindow* GLFWwindowPtr; 
	static Camera* cam;

	static void Stop(); 
	static void Update(); 
};

