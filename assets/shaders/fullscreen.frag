#version 460 core

uniform sampler2D grayscale_texture;

in vec2 vUV;
out vec4 fragColor;

void main()
{
    vec3 color = texture(grayscale_texture, vUV).rgb;

    float gray = dot(color, vec3(0.299, 0.587, 0.114));

    fragColor = vec4(gray, gray, gray, 1.0);
}
