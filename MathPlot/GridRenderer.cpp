#include "GridRenderer.h"

#include "Plot.h"
#include "Program.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

GridRenderer::GridRenderer():
	shader(R"(.\shaders\grid.vert)", R"(.\shaders\grid.frag)")
{
    const double vertices[] = {
    1.0, 1.0, 
    -1.0, -1.0, 
    -1.0, 1.0, 
    -1.0, -1.0, 
    1.0, 1.0, 
    1.0, -1.0,
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), static_cast<void*>(nullptr));

    shader.use();
    transformLoc = glGetUniformLocation(shader, "inv_transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), glm::vec3(10.0f))));

    scaleLoc = glGetUniformLocation(shader, "scale");
    glUniform1f(scaleLoc, 1.0f);

    lineWidthXLoc = glGetUniformLocation(shader, "lineWidthX");
    glUniform1f(lineWidthXLoc, 0.02f);

    lineWidthYLoc = glGetUniformLocation(shader, "lineWidthY");
    glUniform1f(lineWidthYLoc, 0.02f);
}

void GridRenderer::draw()
{
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GridRenderer::updatePlot(const Plot& plot)
{
    shader.use();
    auto inverseTransform = glm::inverse(plot.getTransformMatrix());

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(inverseTransform));

    double logScale = std::log10(std::max(plot.getWidth(), plot.getHeight()));
    glUniform1f(scaleLoc, static_cast<float>(logScale));

    double lineWidth = std::min(plot.getWidth(), plot.getHeight()) / 1000;

    // Line needs to be as thick as the size of a pixel in minimum
    double pixelWidth = getProgram()->getWidth();
    double lineWidthX = std::max(lineWidth, plot.getWidth() / pixelWidth);
    glUniform1f(lineWidthXLoc, lineWidthX);

    double pixelHeight = getProgram()->getHeight();
    double lineWidthY = std::max(lineWidth, plot.getHeight() / pixelHeight);
    glUniform1f(lineWidthYLoc, lineWidthY);
}
