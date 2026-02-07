#version 450 core

layout(location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 u_Model;

void main()
{
    gl_Position = lightSpaceMatrix * u_Model * vec4(aPos, 1.0);
}
