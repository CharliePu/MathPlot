#include "LineRenderer.h"

#include <array>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include "Point.h"

LineRenderer::LineRenderer():
shader(R"(.\shaders\line.vert)", R"(.\shaders\line.frag)")
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), nullptr);

    transformLoc = glGetUniformLocation(shader, "transMat");
}

void LineRenderer::draw()
{
    glLineWidth(8.0);
    shader.use();
    glBindVertexArray(vao);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(getTransMat()));
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
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * data.size(), data.data(), GL_DYNAMIC_DRAW);
}