#include "Pane.h"
#include <glm/gtc/type_ptr.hpp>

void Pane::clickCallback()
{
}

bool Pane::isClicked(double x, double y)
{
    return false;
}

void Pane::typeCallback(char c)
{
}

void Pane::typeUnfocusCallback()
{
}

void Pane::draw()
{
    shader.use();
    glBindVertexArray(vao);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMat));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
}

void Pane::setPosition(double x, double y)
{
    this->x = x;
    this->y = y;

    transformMat = glm::translate(glm::mat4(1.0), glm::vec3(x, y, 0));
}

void Pane::update(int windowWidth, int windowHeight)
{
}

Pane::Pane(double x, double y, double width, double height): width(width), height(height),
shader(R"(.\shaders\pane.vert)", R"(.\shaders\pane.frag)")
{
    setPosition(x, y);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), nullptr);

    transformLoc = glGetUniformLocation(shader, "transMat");
}

void Pane::setSize(double width, double height)
{
    this->width = width;
    this->height = height;
    updateVertices();
}

void Pane::updateVertices()
{
    std::vector<double> vertices;
    vertices.push_back(-width / 2.0);
    vertices.push_back(height / 2.0);

    vertices.push_back(width / 2.0);
    vertices.push_back(height / 2.0);

    vertices.push_back(-width / 2.0);
    vertices.push_back(-height / 2.0);

    vertices.push_back(width / 2.0);
    vertices.push_back(-height / 2.0);

    vertexCount = vertices.size() / 2;


    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
}
