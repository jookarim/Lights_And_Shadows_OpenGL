#version 450 core

in vec2 texCoords;
in vec3 normal;

out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0, 0.0, 1.0, 1.0);
}