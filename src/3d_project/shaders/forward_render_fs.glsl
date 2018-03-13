#version 400

uniform sampler2D textureSampler;

in Data
{
	vec2 uv;
	vec4 normal;
	vec4 pos;
} DataIn;

out vec3 fragment_color;

/*
float dot(vec4 v1, vec4 v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

const vec4 lightPosition = vec4(0.f, 0.f, -10.f, 0.f);
*/
void main () 
{
/*
	float modifier = dot(DataIn.normal, normalize(DataIn.pos - lightPosition));

	if(modifier > 0.f)
		modifier += 50.f / 255.f;

	if (modifier > 1.f)
		modifier = 1.f;
*/
	fragment_color = texture(textureSampler, DataIn.uv).rgb;// * modifier;
}