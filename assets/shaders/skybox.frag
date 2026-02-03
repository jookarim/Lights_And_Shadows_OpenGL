#version 450 core

in vec3 vTexDir;

layout(binding = 0) uniform samplerCube cubemap;

out vec4 fragColor;

void main()
{
	fragColor = texture(cubemap, vTexDir);
}