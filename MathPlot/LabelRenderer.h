#pragma once

#include "Shader.h"
#include "Font.h"
#include "Program.h"
#include "Label.h"

#include "Subscriber.h"

struct LabelRenderInfo
{
    double boundX0, boundY0, boundX1, boundY1;

    std::vector<double> vertices;
};

class LabelRenderer: public Subscriber
{
public:
    LabelRenderer();

	void draw();

    std::shared_ptr<Label> within(double x, double y);

    void addLabel(const std::shared_ptr<Label> &label);
    void removeLabel(const std::shared_ptr<Label>& label);

    void update(const std::shared_ptr<Publisher>& label) override;
    void update();

private:

    void sendVertices();

    void generateVertices(const std::shared_ptr<Label>& label);

    std::unordered_map<std::shared_ptr<Label>, LabelRenderInfo> labelInfos;

    std::unique_ptr<Font> font;

    Shader shader;

    unsigned int vbo, vao, textureId;

    int vertexCount;

    // Unused for now
    glm::mat4 transformMat = glm::mat4(1.0);
};