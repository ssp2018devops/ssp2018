#version 400

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;

in Data
{
	vec2 uv;
	vec4 normal;
	vec3 pos;
	vec3 tangent;
	vec3 bitangent;
} DataIn;

layout (location = 0) out vec3 pos;
layout (location = 1) out vec4 diffuse;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec3 texCoord;
layout (location = 4) out vec3 objectId;
layout (location = 5) out vec3 ambient;
layout (location = 6) out vec3 specular;

uniform uint id;
uniform vec3 kA;
uniform vec3 kD;
uniform vec3 kS;

void main ()
{
    ambient = kA;
    specular = kS;

	pos = DataIn.pos;


    diffuse = vec4(texture(textureSampler, DataIn.uv).xyz, 1.f);
	texCoord = vec3(DataIn.uv, 0.f);

	mat3 fromTriangleSpace = mat3(DataIn.tangent, DataIn.bitangent, DataIn.normal.xyz);
	vec3 normalSample = texture(normalSampler, DataIn.uv).xyz * 2.f - 1.f;
	normal = fromTriangleSpace * normalSample;

	objectId = vec3(id, 0, 0);
}
