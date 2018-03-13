#version 400

layout (location = 0) out vec3 pos;
layout (location = 1) out vec4 diffuse;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec3 texCoord;
layout (location = 5) out vec3 ambient;
layout (location = 6) out vec3 specular;

void main ()
{
	pos = vec3(0,0,0);
	texCoord = vec3(0,0,0);
	normal = vec3(0,0,1);

    ambient = vec3(0.1f, 0.1f, 0.1f);
    diffuse = vec4(0.f, 1.f, 0.f, 1.f);
    specular = vec3(1.f, 1.f, 1.f);
}
