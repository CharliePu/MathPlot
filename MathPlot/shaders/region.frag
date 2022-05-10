#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;

in vec2 TexCoords;

void main()
{
    vec4 color = texture(texture1, TexCoords);
    if (color == vec4(0.0, 0.0, 0.0, 1.0))
        discard;
    if (color == vec4(1.0))
        FragColor = color;
    else
        FragColor = vec4(color.xyz, 0.8);
}