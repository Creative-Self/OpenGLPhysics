#version 430 

out vec4 fragColor; 

//in float red; // old color stuff 
//in float green; 

in vec3 fragLoc; 
in vec3 fragNormal; 
in vec2 fragUV; 

layout(location = 4) uniform vec3 camLoc; 
layout(location = 5) uniform vec3 lightPos; 

uniform sampler2D myTexture; 

void main(){

	vec3 L = normalize(fragLoc - lightPos);
	vec3 V = normalize(fragLoc - camLoc); 
	vec3 H = normalize(L+V);
	vec3 N = normalize(fragNormal); 

	float ambient = .4f; 
	float diffuse = .2f * max(dot(L, N), 0); 
	float specular = 1.5f * pow(max(dot(H, N), 0), 10.0f); 
	float brightness = ambient + diffuse + specular; 	

	// float bloo = (tan(0)+1 * 3.14)/2;	
	// gl_FragColor = vec4(red * brightness,green * brightness ,bloo * brightness,1);
	
	vec4 colorz = texture(myTexture, fragUV);
	gl_FragColor = vec4( (colorz.rgb) * brightness, colorz.a);

}