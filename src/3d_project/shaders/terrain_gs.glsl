#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Data
{
	vec2 texCoord;
	vec3 worldPos;
} DataIn[3];

out Data
{
	vec2 texCoord;
	vec3 worldPos;
	vec3 tangent;
	vec3 bitangent;
	vec3 normal;
} DataOut;

void main()
{
	vec3 edge1 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
	vec3 edge2 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);

	if(dot(normalize(vec3(gl_in[0].gl_Position) - vec3(0, 0, -1)), normalize(cross(edge1, edge2))) < 0.f)
		return;

	edge1 = DataIn[1].worldPos - DataIn[0].worldPos;
	edge2 = DataIn[2].worldPos - DataIn[0].worldPos;

	vec2 deltaUv1 = (DataIn[1].texCoord - DataIn[0].texCoord);// / float(image_width);
	vec2 deltaUv2 = (DataIn[2].texCoord - DataIn[0].texCoord);// / float(image_width);

	float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);

	DataOut.tangent = (edge1 * deltaUv2.y - edge2 * deltaUv1.y) * r;
	DataOut.bitangent = (edge2 * deltaUv1.x - edge1 * deltaUv2.x) * r;
	DataOut.normal = cross(edge1, edge2);



	for(int i = 0; i < gl_in.length(); i++)
	{
		DataOut.worldPos = DataIn[i].worldPos;
		DataOut.texCoord = DataIn[i].texCoord;
		gl_Position = gl_in[i].gl_Position;

		EmitVertex();
	}
	EndPrimitive();
}
