#version 330 core
layout (location = 0) in vec2 pos;

out vec2 coord;

uniform mat4 inv_transform;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);

    coord = (inv_transform * vec4(pos, 0.0, 1.0)).xy;
}