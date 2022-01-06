#pragma once
#include "Renderer.h"
class RegionRenderer :
    public Renderer
{
public:
    RegionRenderer();
    virtual void draw() override;
    void updateData(const std::vector<unsigned char>& data, size_t width, size_t height);
private:
    Shader shader;
    unsigned int vao, vbo, textureId, transformLoc;
};

