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

    unsigned int vao, vbo;
    Shader shader;
};

