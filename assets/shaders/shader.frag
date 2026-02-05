#version 450 core

in vec2 texCoords;
in mat3 TBN;
in vec4 worldPos;

out vec4 fragColor;

#define MAX_DIR_LIGHTS 16
#define MAX_POINT_LIGHTS 32

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D normal;

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
};

struct PointLight
{
    vec3 ambient;
    float pad0;
    vec3 diffuse;
    float pad1;
    vec3 specular;
    float pad2;
    vec3 position;
    float linear;
    float quadric;
    float constant;
    float pad3;
};

layout(binding = 2) readonly buffer DirLights
{
    DirectionalLight dirLights[MAX_DIR_LIGHTS];
};

layout(binding = 3) readonly buffer PointLights
{
    PointLight pointLights[MAX_POINT_LIGHTS];
};

uniform vec3 viewPos;
uniform int dirLightsCount;
uniform int pointLightsCount;

vec3 calculateDirLight(DirectionalLight dirLight, vec3 viewPos, vec3 normal,vec3 albedoColor)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 viewDir = normalize(viewPos - worldPos.xyz);

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * albedoColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32.0);
    vec3 specular = spec * dirLight.specular;

    vec3 ambient = dirLight.ambient * albedoColor;
    
    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight pointLight, vec3 viewPos, vec3 normal, vec3 albedoColor)
{
    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(pointLight.position - worldPos.xyz);
    vec3 viewDir = normalize(viewPos - worldPos.xyz);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pointLight.diffuse * albedoColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * pointLight.specular;

    vec3 ambient = pointLight.ambient * albedoColor;

    float distance = length(pointLight.position - worldPos.xyz);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadric * distance * distance);

    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 albedoColor = texture(albedo, texCoords).rgb;
    vec3 lighting = vec3(0.0);

    //transform from texture space to world space and make from -1 to 1 to make left, down and backward 
    vec3 normalMap = normalize(TBN * (texture(normal, texCoords).rgb * 2.0 - 1.0)); 

    if (dirLightsCount <= MAX_DIR_LIGHTS)
    {
        for (int i = 0; i < dirLightsCount; ++i)
        {
            lighting += calculateDirLight(dirLights[i], viewPos, normalMap, albedoColor);
        }
    }
   
    if (pointLightsCount <= MAX_POINT_LIGHTS)
    {
        for (int i = 0; i < pointLightsCount; ++i)
        {
            lighting += calculatePointLight(pointLights[i], viewPos, normalMap, albedoColor);
        }
    }

    vec3 color = pow(lighting, vec3(1.0 / 2.2));
    fragColor = vec4(color, 1.0);
}
