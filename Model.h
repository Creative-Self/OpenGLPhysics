#pragma once
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include "Engine.h"
#include <string>
#include <fstream>
#include <sstream>

using glm::vec3; 
using glm::vec2; 
using std::vector;
using std::string; 

class Model
{
public:
	Model();
	Model(int cType); 
	int colliderType; // 0 colliderless // 1 box // 2 sphere 
	bool buffer(string objFile); 
	void render();
	vec3 modelHalfsize; 
	~Model();

private: 
	GLuint vertArr;
	GLuint vertBuf;
	unsigned int vertCount;
};

struct Vertex {
	vec3 location; 
	vec2 uv; 
	vec3 normal; 
};

struct VertInd {
	unsigned int locInd; 
	unsigned int uvInd;
	unsigned int normInd;
};

