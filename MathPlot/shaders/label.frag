#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(texture(texture1, TexCoords).r);
    // FragColor = vec4(0.0, TexCoords, 0.8);
}