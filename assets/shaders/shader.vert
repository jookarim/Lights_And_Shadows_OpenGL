#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

out vec2 texCoords;
out mat3 TBN;
out vec4 worldPos;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_Norm;

void main()
{
    texCoords = aTexCoords;

    vec3 normal = normalize(u_Norm * aNormal);
    vec3 tangent = normalize(u_Norm * aTangent);
    vec3 bitangent = normalize(cross(normal, tangent));

    TBN = mat3(tangent, bitangent, normal);

    worldPos = u_Model * vec4(aPos, 1.0);
    gl_Position = u_MVP * vec4(aPos, 1.0);
}
