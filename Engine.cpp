#include "Engine.h"

Timer Engine::time; 
GLFWwindow* Engine::GLFWwindowPtr = nullptr;
Camera* Engine::cam = new Camera();

Engine::Engine()
{ 
}


Engine::~Engine()
{
}

void Engine::Start()
{
	Engine::GLFWwindowPtr = glfwCreateWindow(800, 600, "Andrew Min DSA1 Engine", NULL, NULL); // open up a window 
	cam->start(); // start camera up 
}

void Engine::Stop()
{
}

void Engine::Update()
{
	//Update Camera 
	cam->control();
	cam->upload();

	time.update(); //Update time 
}
