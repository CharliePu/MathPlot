#pragma once
#include "Renderer.h"

struct Point;

class LineRenderer :
    public Renderer
{
public:
    LineRenderer();
    virtual void draw() override;
    virtual void updateData(const std::vector<std::vector<double>>& data) override;
private:
    void processRect(const std::array<Point, 4>& points);
    void identifyLineSegment(std::array<Point, 3> points, std::vector<double>& vertices);
    Point findZeroPoint(Point p1, Point p2);

    unsigned int vao, vbo;
    Shader shader;
};

