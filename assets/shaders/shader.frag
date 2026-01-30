#version 450 core

in vec2 texCoords;
in vec3 normal;

out vec4 fragColor;

layout(binding = 1) uniform sampler2D albedo;

void main()
{
	fragColor = texture(albedo, texCoords);
}