#include "GridRenderer.h"

#include "Plot.h"

GridRenderer::GridRenderer():
	shader(R"(.\shaders\grid.vert)", R"(.\shaders\grid.frag)")
{
	constexpr double vertices[] = {
        -1.0, 0.0, 1.0, 0.0,
        0.0, -1.0, 0.0, 1.0
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), static_cast<void*>(nullptr));
}

void GridRenderer::draw()
{
    glLineWidth(2.0);
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 4);
}

void GridRenderer::updatePlot(const Plot& plot) const
{
	const double xPos = (0 - plot.getXMin()) / plot.getWidth() * 2.0 - 1.0;
	const double yPos = (0 - plot.getYMin()) / plot.getHeight() * 2.0 - 1.0;

	const double vertices[] = {
    -1.0, yPos, 1.0, yPos,
    xPos, -1.0, xPos, 1.0
    };

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
