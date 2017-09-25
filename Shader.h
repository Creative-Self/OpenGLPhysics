#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>


using std::string;

class Shader
{
public:
	Shader();
	~Shader();

	GLuint program, vprogram, fprogram; 
	string filenamev = "shaders/vShader.glsl";
	string filenamef = "shaders/fShader.glsl";

	bool load(); 
	bool compile(GLenum shaderType); 
	void use(); 
	void unload(); 
};

