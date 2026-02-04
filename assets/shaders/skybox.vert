#version 450 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_VP;

out vec3 vTexDir;

void main()
{
	vec4 pos = u_VP * vec4(aPos, 1.0);
	
	vTexDir = aPos;
	gl_Position = pos.xyww;
}