#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

out vec2 texCoords;
out vec3 normal;

uniform mat4 u_MVP;

void main()
{
	texCoords = aTexCoords;
	normal = aNormal;

	gl_Position = u_MVP * vec4(aPos, 1.0);
}