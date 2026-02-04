#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

out vec2 texCoords;
out vec3 normal;

uniform mat4 u_MVP;
uniform mat4 u_Model;

out vec4 worldPos;

uniform mat3 u_Norm;

void main()
{
	texCoords = aTexCoords;

	normal = normalize(u_Norm * aNormal);

	worldPos = u_Model * vec4(aPos, 1.0);

	gl_Position = u_MVP * vec4(aPos, 1.0);
}