#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in Data
{
	vec2 uv;
	vec4 normal;
	vec4 pos;
} DataIn[3];

out Data
{
	vec2 uv;
	vec4 normal;
	vec4 pos;
} DataOut;


vec4 crossProduct(vec4 v1, vec4 v2)
{
	return vec4
	(
		v1[1] * v2[2] - v1[2] * v2[1],
		v1[2] * v2[0] - v1[0] * v2[2],
		v1[0] * v2[1] - v1[1] * v2[0],
		v1[3]
	);
}


uniform mat4 MVP;

void main()
{
	
	vec4 edge1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
	vec4 edge2 = gl_in[2].gl_Position - gl_in[0].gl_Position;

	DataOut.normal = MVP * normalize(crossProduct(edge1, edge2));

	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = MVP * gl_in[i].gl_Position;
		DataOut.pos = gl_Position;
		DataOut.uv = DataIn[i].uv;

		EmitVertex();
	}
	EndPrimitive();
/*	
	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = MVP * gl_in[i].gl_Position + DataOut.normal / 2.f;
		DataOut.pos = gl_Position;
		DataOut.uv = DataIn[i].uv;

		EmitVertex();
	}
	EndPrimitive();
*/
}