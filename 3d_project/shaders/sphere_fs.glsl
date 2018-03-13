#version 400

layout (location = 0) out vec3 pos;
layout (location = 1) out vec4 diffuse;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec3 texCoord; 

void main () 
{
	pos = vec3(0,0,0);
	diffuse = vec4(255, 255, 255, 0);
	texCoord = vec3(0,0,0);
	normal = vec3(0,0,1);
}