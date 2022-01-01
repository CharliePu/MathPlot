#include "Renderer.h"
#include <algorithm>
#include <array>

#include <glad/glad.h>

#include "Point.h"

Renderer::Renderer(int width, int height): 
    regionShader(R"(.\shaders\region.vert)", R"(.\shaders\region.frag)"),
    lineShader(R"(.\shaders\line.vert)", R"(.\shaders\line.frag)")
{
    double quadVertices[] = {
    -1.0,  1.0,  0.0, 1.0,
    -1.0, -1.0,  0.0, 0.0,
     1.0, -1.0,  1.0, 0.0,
    -1.0,  1.0,  0.0, 1.0,
     1.0, -1.0,  1.0, 0.0,
     1.0,  1.0,  1.0, 1.0
    };

    glGenVertexArrays(1, &quadVao);
    glGenBuffers(1, &quadVbo);
    glBindVertexArray(quadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (void*)(2 * sizeof(double)));

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);



    glLineWidth(4.0);

    glGenVertexArrays(1, &lineVao);
    glGenBuffers(1, &lineVbo);
    glBindVertexArray(lineVao);
    glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (void*)0);

}

void Renderer::draw()
{
    glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (this->shouldDrawRegions)
    {
        drawRegions();
    }
    drawLines();
}

void Renderer::updateData(const std::vector<std::vector<double>>& data, bool inequality)
{
    if (data.size() == 0 || data.front().size() == 0)
    {
        return;
    }

    this->shouldDrawRegions = inequality;
    if (inequality)
    {
        updateRegionData(data);
    }

    updateLineData(data);
}

void Renderer::updateRegionData(const std::vector<std::vector<double>>& data)
{
    std::vector<unsigned char> flattenedData;
    flattenedData.reserve(data.size() * data.front().size());
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data.front().size(); j++)
        {
            flattenedData.push_back(static_cast<unsigned char>(data[i][j] * 255));
        }

        // 4-byte alignment
        while (flattenedData.size() % 4 != 0)
        {
            flattenedData.push_back(0);
        }
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.front().size(), data.size(), 0, GL_RED, GL_UNSIGNED_BYTE, &flattenedData[0]);
}

void Renderer::updateLineData(const std::vector<std::vector<double>>& data)
{
    double lineVertices[] = {
        -1.0, -1.0,
        1.0, 1.0
    };
    glBindVertexArray(lineVao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_DYNAMIC_DRAW);
}

void Renderer::drawRegions()
{
    regionShader.use();
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindVertexArray(quadVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::drawLines()
{
    lineShader.use();
    glBindVertexArray(lineVao);
    glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::processRect(const std::array<Point, 4>& points)
{
    double average = 0;
    for (auto& point : points)
    {
        average += point.value;
    }
    average /= points.size();

    
}

void Renderer::identifyLineSegment(std::array<Point, 3> points, std::vector<double> &vertices)
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

Point Renderer::findZeroPoint(Point p1, Point p2)
{
    Point p((p2 - p1)* (0.0 - p1.value) / (p2.value - p1.value));
    p.value = 0;
    return p;
}
