#version 450 core

in vec2 texCoords;
in vec3 normal;
in vec4 worldPos;

out vec4 fragColor;

#define MAX_DIR_LIGHTS 16

layout(binding = 0) uniform sampler2D albedo;

struct DirectionalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

layout(binding = 1) buffer DirLights
{
    DirectionalLight dirLights[MAX_DIR_LIGHTS];
};

uniform vec3 viewPos;

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

void main()
{
    vec3 albedoColor = texture(albedo, texCoords).rgb;
    vec3 lighting = vec3(0.0);

    lighting += calculateDirLight(dirLights[0], viewPos, normal, albedoColor);

    vec3 color = pow(lighting, vec3(1.0 / 2.2));
    fragColor = vec4(color, 1.0);
}
