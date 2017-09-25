#pragma once
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <string>
//#include "GLDebug.h"
//#include "GLError.h"

#include "Camera.h"
#include "Model.h"
#include "Engine.h"
#include "Shader.h"

class Texture
{
public:
	Texture();
	Texture(string path); 
	void bindTexture(); 
	void unloadTexture();
	unsigned int texID; 
	~Texture();
};

