#version 400

uniform sampler2D textureSampler;


in Data
{
	vec2 uv;
	vec3 normal;
	vec3 pos;
} DataIn;

layout (location = 0) out vec3 pos;
layout (location = 1) out vec4 diffuse;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec3 texCoord;
layout (location = 5) out vec3 ambient;
layout (location = 6) out vec3 specular;

void main ()
{
    ambient = vec3(0.1f, 0.1f, 0.1f);
    specular = vec3(1.f, 1.f, 1.f);

	vec4 diffuseSample = texture(textureSampler, DataIn.uv);
	if(diffuseSample.a < 0.5f)
		discard;

	pos = DataIn.pos;
	diffuse = vec4(diffuseSample.xyz, 0.f);
	texCoord = vec3(DataIn.uv, 0.f);

	normal = DataIn.normal;
}
