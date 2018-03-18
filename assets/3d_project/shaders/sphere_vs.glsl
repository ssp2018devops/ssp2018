#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in float sphere_radius;


out Data
{
	float sphere_radius;
} DataOut;



void main() 
{
	DataOut.sphere_radius = sphere_radius;
	gl_Position = vec4(vertex_position, 1.0f);
}