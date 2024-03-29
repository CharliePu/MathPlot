#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 transMat;

void main()
{
    gl_Position = transMat * vec4(pos, 0.0, 1.0);
    TexCoords = texCoords;
}