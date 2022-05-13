#pragma once
#include "Widget.h"

#include <string>

#include "glm/mat4x4.hpp"

#include "Shader.h"
#include "Font.h"

class Label :
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

    Label();
    Label(double x, double y, const std::string& text);

    void setText(const std::string& text);

private:
    std::string text;
    std::unique_ptr<Font> font;


    Shader shader;
    double x, y;

    unsigned int vbo, vao, textureId;

    int transformLoc, vertexCount;
    glm::mat4 transformMat;

    std::vector<double> vertices;
};

