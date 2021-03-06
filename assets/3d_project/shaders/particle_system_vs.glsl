#version 400
layout(location = 0) in vec3 vertex_position;




out Data
{
	vec3 pos;
} DataOut;


uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertex_position, 1.0f);
    DataOut.pos = vertex_position;
}
