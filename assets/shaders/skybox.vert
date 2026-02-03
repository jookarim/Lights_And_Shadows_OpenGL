#version 450 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 vTexDir;

void main()
{
	mat4 view = mat4(mat3(u_View));
	vec4 pos = u_Proj * view * vec4(aPos, 1.0);
	
	vTexDir = aPos;
	gl_Position = pos.xyww;
}