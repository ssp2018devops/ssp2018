#version 400

layout(location = 0) in float vertex_height;

uniform mat4 MVP;
uniform mat4 modelTransform;
uniform int image_width;

out Data
{
	vec2 texCoord;
	vec3 worldPos;
} DataOut;

void main()
{
	int index = gl_VertexID - (gl_VertexID + 2) / (2 * image_width + 2) - gl_VertexID / (2 * image_width + 2);
	index = int(mod(index, 2)) * image_width + index / 2;

	vec2 heightmapCoord = vec2(mod(index, image_width), index / image_width);
	DataOut.texCoord = heightmapCoord / float(image_width);

//	vec4 position = vec4(heightmapCoord.x - image_width, vertex_height - 255.f / 2.f, heightmapCoord.y, 1.f);
    vec4 position = vec4(heightmapCoord.x, vertex_height, heightmapCoord.y, 1.f);

	gl_Position = MVP * position;
	DataOut.worldPos = vec3(modelTransform * position);
}
