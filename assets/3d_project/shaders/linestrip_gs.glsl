#version 400

layout(lines) in;
layout(line_strip, max_vertices = 2) out;


void main()
{
	for(int i = 0; i < 2; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}