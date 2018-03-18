#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 8) out;

in Data
{
	vec3 pos;
} DataIn[1];

out Data
{
	vec2 uv;
	vec3 normal;
	vec3 pos;
} DataOut;


uniform mat4 MVP;

uniform vec3 cameraUp;
uniform vec3 cameraRight;

void main()
{
    vec4 camUp = vec4(normalize(cameraUp), 0.f);
    vec4 camRight = vec4(normalize(cameraRight), 0.f);
    camUp /= 4.f;
    camRight /= 4.f;

    vec4 p = vec4(DataIn[0].pos, 1.f);

	vec4 pos[4];
	pos[0] = p + (camUp - camRight);
	pos[1] = p + (-camUp - camRight);
	pos[2] = p + (camUp + camRight);
	pos[3] = p + (-camUp + camRight);

	vec2 uv[4];
	uv[0] = vec2(0.f, 0.f);
	uv[1] = vec2(0.f, 1.f);
	uv[2] = vec2(1.f, 0.f);
	uv[3] = vec2(1.f, 1.f);

    DataOut.normal = cross(camRight.xyz, camUp.xyz);

	for(int i = 0; i < 4; i++)
	{
		DataOut.pos = vec3(pos[i]);
		DataOut.uv = uv[i];
		gl_Position = MVP * pos[i];

		EmitVertex();
	}
	EndPrimitive();
}
