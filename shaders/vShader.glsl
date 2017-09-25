#version 430

layout (location = 0) in vec3 position;				//attribute: "in" (vert shader)
layout (location = 1) uniform mat4 worldView;	//uniform
layout (location = 2) in vec3 normal;				//models normal 
layout (location = 3) in vec2 modelUV; 
layout (location = 6) uniform mat4 modelPosition; 

//out float red;			// old color stuff 
//out float green; 

out vec3 fragLoc; 
out vec3 fragNormal; 
out vec2 fragUV; 

void main(){

	//red = (position.x +1)/2; 
	//green = (position.y +1)/2; 

	gl_Position = worldView * modelPosition * vec4(position, 1);
	
	fragLoc = position; 
	fragNormal = normal; 
	fragUV = modelUV;
}