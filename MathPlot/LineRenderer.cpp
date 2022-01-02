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
    glDrawArrays(GL_LINES, 0, vertexCount);
}

void LineRenderer::updateData(const std::vector<double>& data)
{
    //std::vector<double> lineVertices = {
    //-1.0, -1.0,
    //1.0, 1.0
    //};

    vertexCount = data.size() / 2;

    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * data.size(), data.data(), GL_DYNAMIC_DRAW);
}