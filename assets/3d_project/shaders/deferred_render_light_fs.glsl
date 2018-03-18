//#version 330

uniform sampler2D pos;
uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D ambient;
uniform sampler2D specular;


const vec4 lightPosition = vec4(0.f, 0.f, 1000.f, 1.f);

out vec3 fragment_color;

uniform vec2 windowSize;
uniform vec3 eyePosition;

void main ()
{
	vec2 normalizedFragCoord = gl_FragCoord.xy / windowSize;


    vec3 ambientTerm = texture(ambient, normalizedFragCoord).rgb;

    vec3 surfacePosition = texture(pos, normalizedFragCoord).xyz;
	vec3 surfaceToLightDirection = normalize(lightPosition.xyz - surfacePosition);
	vec3 surfaceNormal = normalize(texture(normal, normalizedFragCoord).xyz);

	vec4 diffuseSample = texture(diffuse, normalizedFragCoord);
	float intensity = max(dot(surfaceNormal, surfaceToLightDirection), 0.f);
	vec3 diffuseTerm = diffuseSample.rgb * intensity;

    vec3 specularTerm = vec3(0.f);
	if(intensity > 0.f)
	{
        vec3 reflectionDirection = normalize(reflect(-surfaceToLightDirection, surfaceNormal));
        float shininess = 200.f;
        vec3 specularSample = texture(specular, normalizedFragCoord).rgb;
        specularTerm =  specularSample * pow(max(dot(normalize(eyePosition - surfacePosition), reflectionDirection), 0.f), shininess);
	}

    vec3 illumination = ambientTerm + diffuseTerm + specularTerm - diffuseSample.rgb;

	fragment_color = diffuseSample.rgb + diffuseSample.a * illumination;

}
