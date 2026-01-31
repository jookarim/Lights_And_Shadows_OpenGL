#version 450 core

const vec2 quadPoses[6] = vec2[](
    vec2(-1.0, 1.0),
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(1.0, 1.0)
    );

const vec2 quadTexCoords[6] = vec2[](
    vec2(0.0, 1.0),
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0)
    );

out vec2 vUV;

void main()
{
    int id = gl_VertexID;

    gl_Position = vec4(quadPoses[id], 0.0, 1.0);
    vUV = quadTexCoords[id];
}
