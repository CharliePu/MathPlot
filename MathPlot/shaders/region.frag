#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;

in vec2 TexCoords;

void main()
{
    FragColor = vec4(vec3(texture(texture1, TexCoords).r), 1.0);
}