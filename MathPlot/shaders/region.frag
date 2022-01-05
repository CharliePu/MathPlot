#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;

in vec2 TexCoords;

void main()
{
    FragColor = vec4(texture(texture1, TexCoords).rgb, 1.0);
}