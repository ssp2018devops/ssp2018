#version 400

uniform samplerCube textureSampler;
uniform sampler2D normalSampler;

in Data
{
	vec2 uv;
	vec4 normal;
	vec3 pos;
	vec3 tangent;
	vec3 bitangent;
	vec3 localPos;
} DataIn;

layout (location = 0) out vec3 pos;
layout (location = 1) out vec4 diffuse;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec3 texCoord;
layout (location = 4) out vec3 objectId;
layout (location = 5) out vec3 ambient;
layout (location = 6) out vec3 specular;

uniform uint id;

uniform vec3 meshCenter;

void main ()
{
    ambient = vec3(0.1f, 0.1f, 0.1f);
    specular = vec3(1.f, 1.f, 1.f);

	pos = DataIn.pos;
	diffuse = vec4(texture(textureSampler, DataIn.pos - meshCenter).xyz, 0.f);
//    diffuse = vec4(texture(textureSampler, DataIn.localPos).xyz, 0.f);
//	diffuse = vec4(.5f, 0.5f, 0.5f, 255.f);
//    diffuse = vec4(texture(textureSampler, DataIn.localPos).xyz * 0 + vec3(.5f, 0.5f, 0.5f), 255.f);
	texCoord = vec3(DataIn.uv, 0.f);

//	mat3 fromTriangleSpace = mat3(DataIn.tangent, DataIn.bitangent, DataIn.normal);
//	vec3 normalSample = texture(normalSampler, DataIn.uv).xyz * 2.f - 1.f;
//	normal = fromTriangleSpace * normalSample;
    normal = vec3(DataIn.normal);

	objectId = vec3(id, 0, 0);
}
