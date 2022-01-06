#pragma once
#include "Renderer.h"

struct Point;

class LineRenderer :
    public Renderer
{
public:
    LineRenderer();
    virtual void draw() override;
    void updateData(const std::vector<double>& data);
private:
    size_t vertexCount;
    unsigned int vao, vbo, transformLoc;
    Shader shader;
};

