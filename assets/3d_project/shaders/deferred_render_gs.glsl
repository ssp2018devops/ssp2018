#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Data
{
	vec2 uv;
	vec3 pos;
} DataIn[3];

out Data
{
	vec2 uv;
	vec4 normal;
	vec3 pos;
	vec3 tangent;
	vec3 bitangent;
} DataOut;

void main()
{
	vec3 edge1 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
	vec3 edge2 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);

	if(dot(normalize(vec3(gl_in[0].gl_Position) - vec3(0,0,-1)), normalize(cross(edge1, edge2))) < 0.f)
		return;

	edge1 = DataIn[1].pos - DataIn[0].pos;
	edge2 = DataIn[2].pos - DataIn[0].pos;

	vec2 deltaUv1 = DataIn[1].uv - DataIn[0].uv;
	vec2 deltaUv2 = DataIn[2].uv - DataIn[0].uv;






	DataOut.tangent = normalize(edge1 * deltaUv2.y   - edge2 * deltaUv1.y);
	DataOut.bitangent = normalize(edge2 * deltaUv1.x   - edge1 * deltaUv2.x);
	DataOut.normal = normalize(vec4(cross(edge1, edge2), 0.f));

	for(int i = 0; i < gl_in.length(); i++)
	{
		DataOut.pos = DataIn[i].pos;
		DataOut.uv = DataIn[i].uv;
		gl_Position = gl_in[i].gl_Position;

		EmitVertex();
	}
	EndPrimitive();
}
