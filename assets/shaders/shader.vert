#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

#define MAX_DIR_LIGHTS 16

out vec2 texCoords;
out mat3 TBN;
out vec4 worldPos;
out vec4 fragPosLightSpaces[MAX_DIR_LIGHTS];

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_Norm;
uniform int dirLightsCount;

struct DirectionalLight
{
    vec3 ambient;
    float pad0;
    vec3 diffuse;
    float pad1;
    vec3 specular;
    float pad2;
    vec3 direction;
    float pad3;
    mat4 lightSpaceMatrix;
};

layout(binding = 3) readonly buffer DirLights
{
    DirectionalLight dirLights[MAX_DIR_LIGHTS];
};

void main()
{
    texCoords = aTexCoords;

    vec3 normal = normalize(u_Norm * aNormal);
    vec3 tangent = normalize(u_Norm * aTangent);
    vec3 bitangent = normalize(cross(normal, tangent));

    TBN = mat3(tangent, bitangent, normal);

    worldPos = u_Model * vec4(aPos, 1.0);

    for (int i = 0; i < dirLightsCount; ++i)
    {
        fragPosLightSpaces[i] = dirLights[i].lightSpaceMatrix * worldPos;
    }

    gl_Position = u_MVP * vec4(aPos, 1.0);
}
