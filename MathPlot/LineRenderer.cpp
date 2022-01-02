#include "LineRenderer.h"

#include <array>
#include <algorithm>

#include "Point.h"

LineRenderer::LineRenderer():
shader(R"(.\shaders\line.vert)", R"(.\shaders\line.frag)")
{
    glLineWidth(4.0);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (void*)0);
}

void LineRenderer::draw()
{
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 2);
}

void LineRenderer::updateData(const std::vector<std::vector<double>>& data)
{
    if (data.size() == 0 || data.front().size() == 0)
    {
        return;
    }
    
    double lineVertices[] = {
    -1.0, -1.0,
    1.0, 1.0
    };
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_DYNAMIC_DRAW);
}

void LineRenderer::processRect(const std::array<Point, 4>& points)
{
    double average = 0;
    for (auto& point : points)
    {
        average += point.value;
    }
    average /= points.size();


}

void LineRenderer::identifyLineSegment(std::array<Point, 3> points, std::vector<double>& vertices)
{
    std::sort(points.begin(), points.end());

    size_t onPointIndex = -1, abovePointIndex = -1, belowPointIndex = 0;
    for (size_t i = 0; i < points.size(); i++)
    {
        if (onPointIndex != -1 && points[i].value == 0)
        {
            onPointIndex = i;
        }
        if (abovePointIndex != -1 && points[i].value > 0)
        {
            abovePointIndex = i;
            break;
        }
    }

    size_t belowCount = onPointIndex;
    size_t onCount = abovePointIndex - onPointIndex;
    size_t aboveCount = vertices.size() - abovePointIndex;

    if (aboveCount == 3 || belowCount == 3 || onCount == 3)
    {
        return;
    }

    if (onCount == 1 && (aboveCount == 2 || belowCount == 2))
    {
        return;
    }

    Point p1, p2;

    if (onCount == 2)
    {
        p1 = points[onPointIndex];
        p2 = points[onPointIndex + 1];
    }

    if (onCount == 1 && aboveCount == 1 && belowCount == 0)
    {
        p1 = findZeroPoint(points[abovePointIndex], points[belowPointIndex]);
        p2 = points[onPointIndex];
    }

    if (onCount == 0)
    {
        if (belowCount == 2)
        {
            p1 = findZeroPoint(points[abovePointIndex], points[belowPointIndex]);
            p2 = findZeroPoint(points[abovePointIndex], points[belowPointIndex + 1]);

        }
        if (aboveCount == 2)
        {
            p1 = findZeroPoint(points[belowPointIndex], points[abovePointIndex]);
            p2 = findZeroPoint(points[belowPointIndex], points[abovePointIndex + 1]);
        }
    }

    vertices.push_back(p1.x);
    vertices.push_back(p1.y);
    vertices.push_back(p2.x);
    vertices.push_back(p2.y);
}

Point LineRenderer::findZeroPoint(Point p1, Point p2)
{
    Point p((p2 - p1) * (0.0 - p1.value) / (p2.value - p1.value));
    p.value = 0;
    return p;
}
