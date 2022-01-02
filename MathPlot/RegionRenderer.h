#pragma once
#include "Renderer.h"
class RegionRenderer :
    public Renderer
{
public:
    RegionRenderer();
    virtual void draw() override;
    virtual void updateData(const std::vector<std::vector<double>>& data) override;
private:
    Shader shader;
    unsigned int vao, vbo, textureId;
};

