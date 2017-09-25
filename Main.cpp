#include <iostream>
#include <vector>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include<cmath>

#include "Model.h"
#include "Engine.h"
#include "Shader.h"
#include "Texture.h"

using std::cout; 
using std::vector; 
using std::string;  
using std::endl;

//Structs for objects 
struct RigidBody {
	float mass; 
	vec3 force; 
	vec3 velocity;
};

struct Transform {
	mat4 objectWorld;
	vec3 location;
	vec3 rotation;
	vec3 size;	
	RigidBody objectForce; 
};

struct Object {
	Transform objectTransform;
	Model model;
	Texture modelTexture;
};

float DistanceBetween(vec3 loc1, vec3 loc2) { // Used to calculate distance between two vectors 
	float distance =
		pow(loc1.x - loc2.x, 2.0)
		+ pow(loc1.y - loc2.y, 2.0)
		+ pow(loc1.z - loc2.z, 2.0);
	distance = sqrt(distance);
	return distance;
}

bool isColliding(Object& object1, Object& object2) {

	if (object1.model.colliderType == 1 && object2.model.colliderType == 1) { // box

		vec3 obj1Max = object1.model.modelHalfsize * object1.objectTransform.size + object1.objectTransform.location; 
		vec3 obj2Max = object2.model.modelHalfsize * object2.objectTransform.size + object2.objectTransform.location; 

		vec3 obj1Min = object1.objectTransform.location - object1.model.modelHalfsize * object1.objectTransform.size;
		vec3 obj2Min = object2.objectTransform.location - object2.model.modelHalfsize * object2.objectTransform.size;

		//Checks 
		if (obj2Min.x > obj1Max.x) return false; 
		if (obj2Max.x < obj1Min.x) return false; 

		if (obj2Min.y > obj1Max.y) return false; 
		if (obj2Max.y < obj1Min.y) return false;

		if (obj2Min.z > obj1Max.z) return false;
		if (obj2Max.z < obj1Min.z) return false;

		return true; 
	}
	if (object1.model.colliderType == 2 && object2.model.colliderType == 2) { // sphere 

		vec3 radius1 = object1.model.modelHalfsize * object1.objectTransform.size + object1.objectTransform.location;
		vec3 radius2 = object2.model.modelHalfsize * object2.objectTransform.size + object2.objectTransform.location;
		
		float distance = DistanceBetween(object1.objectTransform.location, object2.objectTransform.location);
			
		if (radius1.x + radius2.x <= distance) return false; 
				if (radius1.z + radius2.z <= distance) return false; 

		return true; 
	}
	if (object1.model.colliderType == 1 && object2.model.colliderType == 2) { // box and sphere PARAMETERS IN THAT ORDER 

		vec3 obj1Max = object1.model.modelHalfsize * object1.objectTransform.size + object1.objectTransform.location;
		vec3 obj1Min = object1.objectTransform.location - object1.model.modelHalfsize * object1.objectTransform.size;

		vec3 radius2 = object2.model.modelHalfsize * object2.objectTransform.size + object2.objectTransform.location;

		if (radius2.x > obj1Max.x) return false; 
		if (radius2.y > obj1Max.y) return false;
		if (radius2.z > obj1Max.z) return false;

		if (radius2.x < obj1Min.x) return false; 
		if (radius2.y < obj1Min.y) return false;
		if (radius2.z < obj1Min.z) return false;

		return true; 

	}
	return false; 
}

void UpdateModel(Object& obj) {
	mat4 scale = glm::scale(obj.objectTransform.size);
	mat4 rotation = glm::yawPitchRoll(obj.objectTransform.rotation.y, obj.objectTransform.rotation.x, obj.objectTransform.rotation.z);

	mat4 translation = glm::translate(obj.objectTransform.location);
	obj.objectTransform.objectWorld = translation * rotation * scale;

	glUniformMatrix4fv(6, 1, GL_FALSE, &obj.objectTransform.objectWorld[0][0]);
	obj.modelTexture.bindTexture();
	obj.model.render();
}

vec3 RotateAroundCircle(Object& obj, vec3 centerPoint, float speed) {

	float rotBy = speed * 45; 
	vec3 pos = obj.objectTransform.location; 

	float angle = rotBy * (3.1415926 / 180); 

	pos.x = cos(angle) * (pos.x - centerPoint.x) - sin(angle) * (pos.z - centerPoint.z) + centerPoint.x; 
	pos.z = sin(angle) * (pos.x - centerPoint.x) + cos(angle) * (pos.z - centerPoint.z) + centerPoint.z; 
   // pos.y = centerPoint.y; // use to make it follow Y axis 

	return pos; 

}

void CalcForce(Object& obj) {

	vec3 deltaV = obj.objectTransform.objectForce.force * Engine::time.dt / obj.objectTransform.objectForce.mass;

	 obj.objectTransform.objectForce.force *= 0; 

	obj.objectTransform.objectForce.velocity += deltaV;

	obj.objectTransform.location +=
		obj.objectTransform.objectForce.velocity * Engine::time.dt;
}

void ApplyForce(Object& obj, vec3 force) {
	obj.objectTransform.objectForce.force += force;
}

int main() { 
	// Initial setup 
	Engine gameEngine = Engine();
	srand((unsigned int)time(0));
	if (glfwInit() == GL_FALSE) {
		return -1; 
	}

	gameEngine.Start();

	if (Engine::GLFWwindowPtr != nullptr) { glfwMakeContextCurrent(Engine::GLFWwindowPtr);} 
	else { glfwTerminate(); return -1;}
	if (glewInit()!= GLEW_OK){ glfwTerminate(); return -1; }

	glfwSetInputMode(Engine::GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // hide cursor 
	glClearColor(.5f, 0.0f, .5f, 1.0f); // background color 
	glEnable(GL_DEPTH_TEST);

	/////////////////////////////////////////////////////////////////////////////////////////

	// Models 
	vector<Object> models; 
	float s = 2 * 3.1415926 / 10;
	float little = 2 * 3.1415926 / 40;

	for (int i = 0; i < 10; i++)
	{	
		Object object_instance;
		object_instance.objectTransform.location = vec3(sin(i * s) * 12, 5,cos(i * s) * 12); 
		object_instance.objectTransform.rotation = vec3(0, 0, 0);

	
		if (i % 2 == 0) {
			object_instance.model = Model(1);
			object_instance.objectTransform.size = vec3(1, 1, 1);
			object_instance.modelTexture = Texture("textures/crate.jpg");
			object_instance.model.buffer("models/box.obj");
			object_instance.objectTransform.objectForce.mass = 3.0f;
		}
		else {
			object_instance.model = Model(2);
			object_instance.objectTransform.location.y = -12; 
			object_instance.objectTransform.size = vec3(2, 2, 2);
			object_instance.modelTexture = Texture("textures/asteroid.jpg");
			object_instance.model.buffer("models/sphere.obj");
			object_instance.objectTransform.objectForce.mass = 5.0f;	
		}

		models.push_back(object_instance);
	}

	vector<Object> littleModels; // stars 
	for (int i = 0; i < 40; i++){
		
		Object object_instance;
		float randomY = rand() % 30;
		if (rand() % 100 + 1 > 50) randomY = -randomY; 

		object_instance.objectTransform.location = vec3(sin(i * little) * 35 ,
			randomY,
			cos(i* little) * 35 );

		object_instance.objectTransform.rotation = vec3(0, 0, 0);
		object_instance.model = Model(2);
		object_instance.objectTransform.size = vec3(rand() % 2 + 1, 1, rand() % 2 + 1);
		object_instance.modelTexture = Texture("textures/star.jpg");
		object_instance.model.buffer("models/sphere.obj");
		object_instance.objectTransform.objectForce.mass = 5.0f;
		littleModels.push_back(object_instance);
		
	}

	//Floor 
	Object floor; 
	floor.model = Model(1); 
	floor.objectTransform.location = vec3(0, -20, 0);
	floor.objectTransform.rotation = vec3(0, 0, 0); 
	floor.objectTransform.size = vec3(27, 1, 27); 
	floor.modelTexture = Texture("textures/floor.jpg");
	floor.model.buffer("models/box.obj");
	floor.objectTransform.objectForce.mass = 99.0f;

	Object arena;
	arena.model = Model(1);
	arena.objectTransform.location = vec3(0, 0, 0);
	arena.objectTransform.rotation = vec3(0, 0, 0);
	arena.objectTransform.size = vec3(100, 100, 100);
	arena.modelTexture = Texture("textures/space.png");
	arena.model.buffer("models/sphere.obj");
	arena.objectTransform.objectForce.mass = 0.0f;


	Object testSphere;
	testSphere.model = Model(2);
	testSphere.objectTransform.location = vec3(0, -12, 0);
	testSphere.objectTransform.rotation = vec3(0, 0, 0);
	testSphere.objectTransform.size = vec3(2, 2,2);
	testSphere.modelTexture = Texture("textures/water.jpg");
	testSphere.model.buffer("models/sphere.obj");
	testSphere.objectTransform.objectForce.mass = 99.0f;

	//Shaders 
	Shader testShader = Shader();
	if (testShader.load()) { testShader.use(); }

	//Lighting 
	float lightPosition[3] = { .75, .75, .75 }; // hard coded lighting position 
	glUniform3fv(5, 1, lightPosition); // pass in light pos to fshader

	//Forces 
	vec3 gravity = { 0, -100, 0 };

	//Game loop 
	while (!glfwGetKey(Engine::GLFWwindowPtr, GLFW_KEY_ESCAPE)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gameEngine.Update(); // time and camera 
		arena.objectTransform.location = Engine::cam->camLoc; // we dont talk about this 

		//Update model information
		for (int i = 0; i < models.size(); i++)
		{
			CalcForce(models[i]);			
			UpdateModel(models[i]); 

			//Cubes
			if (i % 2 == 0) {
				if (!isColliding(floor, models[i])) ApplyForce(models[i], gravity);
				else  models[i].objectTransform.objectForce.velocity = -models[i].objectTransform.objectForce.velocity;
			}
			//green guys 
			else {
				models[i].objectTransform.location = RotateAroundCircle(models[i], testSphere.objectTransform.location, .02f);
			}		
		}


		for (int j = 0; j < littleModels.size(); j++) // stars 
		{
			CalcForce(littleModels[j]);
			UpdateModel(littleModels[j]);
			littleModels[j].objectTransform.location = RotateAroundCircle(littleModels[j], testSphere.objectTransform.location, .0007f);
			glm::normalize(littleModels[j].objectTransform.location);

		}

		// render floor 
		UpdateModel(floor); 
		UpdateModel(testSphere); 
		UpdateModel(arena);

		glfwSwapBuffers(Engine::GLFWwindowPtr);
		glfwPollEvents();
	}

	//Unload textures 
	for (int i = 0; i < models.size(); i++)
	{
		models[i].modelTexture.unloadTexture(); 
	}

	glfwTerminate(); 
	return 0; 
}


