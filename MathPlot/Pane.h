#pragma once
#include "Widget.h"
#include "Shader.h"

#include <glm/mat4x4.hpp>

class Pane :
    public Widget
{
public:
    virtual void clickCallback() override;
    virtual bool isClicked(double x, double y) override;
    virtual void typeCallback(char c) override;
    virtual void typeUnfocusCallback() override;
    virtual void draw() override;
    virtual void setPosition(double x, double y) override;
    virtual void update(int windowWidth, int windowHeight) override;

    Pane(double x, double y, double width = 0.2, double height = 0.1);

    void setSize(double width, double height);

private:
    void updateVertices();

    Shader shader;
    double x, y, width, height;

    unsigned int vao, vbo;
    int vertexCount, transformLoc;
    glm::mat4 transformMat;
};

