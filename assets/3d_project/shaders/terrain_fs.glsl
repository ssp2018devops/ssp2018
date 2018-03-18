#version 400

layout (location = 0) out vec3 pos;
layout (location = 1) out vec4 diffuse;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec3 texCoord;
layout (location = 5) out vec3 ambient;
layout (location = 6) out vec3 specular;


uniform sampler2D textureSampler;
//uniform sampler2D normalSampler;

in Data
{
	vec2 texCoord;
	vec3 worldPos;
	vec3 tangent;
	vec3 bitangent;
	vec3 normal;
} DataIn;


void main ()
{
    ambient = vec3(0.1f, 0.1f, 0.1f);
    specular = vec3(1.f, 1.f, 1.f);

	pos = DataIn.worldPos;
	texCoord = vec3(DataIn.texCoord, 0.f);
	diffuse = vec4(texture(textureSampler, DataIn.texCoord).rgb, 1.f);

	//mat3 fromTriangleSpace = mat3(DataIn.tangent, DataIn.bitangent, DataIn.normal);
	//vec3 normalSample = texture(normalSampler, DataIn.texCoord).xyz * 2.f - 1.f;
	//normal = fromTriangleSpace * normalSample;

	normal = DataIn.normal + DataIn.tangent - DataIn.tangent + DataIn.bitangent - DataIn.bitangent;
}
