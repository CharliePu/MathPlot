#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;

in vec2 TexCoords;

void main()
{
    FragColor = vec4(TexCoords, 0.0f, 1.0);
}