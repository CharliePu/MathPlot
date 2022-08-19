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
	void clickCallback() override;
	bool isClicked(double mouseX, double mouseY) override;
	void typeCallback(char c) override;
	void typeUnfocusCallback() override;
	void draw() override;
	void setPosition(double x, double y) override;
	void update(int windowWidth, int windowHeight) override;

    Label();
    Label(double x, double y, const std::string& text);
	void generateVertices();

	void setText(const std::string& text);

private:
    std::string text;
    std::unique_ptr<Font> font;


    Shader shader;
    double x, y, boundX0, boundX1, boundY0, boundY1;

    unsigned int vbo, vao, textureId;

    int transformLoc, vertexCount;
    glm::mat4 transformMat;

    std::vector<double> vertices;
	double windowWidth, windowHeight;
};

