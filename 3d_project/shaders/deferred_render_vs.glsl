#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texCoord;

out Data
{
	vec2 uv;
	vec3 pos;
} DataOut;

uniform mat4 MVP;
uniform mat4 modelTransform;

void main() 
{
	DataOut.uv = vertex_texCoord;
	DataOut.pos = vec3(modelTransform * vec4(vertex_position, 1.f));
	gl_Position = MVP * vec4(vertex_position, 1.0f);
}