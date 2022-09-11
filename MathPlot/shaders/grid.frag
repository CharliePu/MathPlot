#version 330 core
out vec4 FragColor;

in vec2 coord;

uniform float scale;
uniform float lineWidth;

float remainder(float x, float y)
{
    return x - y * floor(x / y);
}

void main()
{
    float s = scale - 1;
    
    float minorScale = pow(10.0f, floor(s));
    float majorScale = pow(10.0f, floor(s) + 1);

    float lineWidth = pow(10.0f, s - 1.9);

    bool minorLine = remainder(coord.x, minorScale) < lineWidth || remainder(coord.y, minorScale) < lineWidth;

    bool majorLine = remainder(coord.x, majorScale) < lineWidth || remainder(coord.y, majorScale) < lineWidth;

    float gridline = float(minorLine) * (1.0f - fract(s)) + float(majorLine) * fract(s);

    float axisLine = float(abs(coord.x) < lineWidth || abs(coord.y) < lineWidth);

    FragColor = vec4(1.0f, 1.0, 1.0, gridline * 0.2 + axisLine * 0.8);
}