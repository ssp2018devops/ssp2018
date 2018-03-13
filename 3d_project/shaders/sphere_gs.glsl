#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

in Data
{
	float sphere_radius;
} DataIn[1];

uniform mat4 MVP;

void main()
{

	vec4 pos[6];
	pos[0] = pos[1] = pos[2] = pos[3] = pos[4] = pos[5] = gl_in[0].gl_Position;

	pos[1].y += DataIn[0].sphere_radius;
	pos[2].x += DataIn[0].sphere_radius;
	pos[3].y -= DataIn[0].sphere_radius;
	pos[4].x -= DataIn[0].sphere_radius;
	pos[5] = pos[1];

	for(int i = 0; i < 6; i++)
	{
		gl_Position = MVP * pos[i];
		EmitVertex();
	}
	EndPrimitive();
}