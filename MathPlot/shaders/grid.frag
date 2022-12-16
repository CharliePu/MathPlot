#version 330 core
out vec4 FragColor;

in vec2 coord;

uniform float scale;
uniform float lineWidthX;
uniform float lineWidthY;

float distanceToFloor(float x, float y)
{
    return x - y * floor(x / y);
}

void main()
{
    float s = scale - 1;
    
    float minorScale = pow(10.0f, floor(s));
    float majorScale = pow(10.0f, floor(s) + 1);

    //float lineWidth = pow(10.0f, s - 2.0);

    bool minorLine = distanceToFloor(coord.x, minorScale) < lineWidthX || distanceToFloor(coord.y, minorScale) < lineWidthY;

    bool majorLine = distanceToFloor(coord.x, majorScale) < lineWidthX || distanceToFloor(coord.y, majorScale) < lineWidthY;

    float gridline = float(minorLine) * (1.0f - fract(s)) + float(majorLine) * fract(s);

    float axisLine = float(abs(coord.x) < lineWidthX || abs(coord.y) < lineWidthY);

    FragColor = vec4(1.0f, 1.0, 1.0, gridline * 0.2 + axisLine * 0.8);
}