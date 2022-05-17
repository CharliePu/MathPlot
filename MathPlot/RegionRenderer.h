#pragma once
#include <vector>

#include "Drawable.h"
#include "Shader.h"
#include "Transformable.h"

class RegionRenderer :
    public Drawable, public Transformable
{
public:
    RegionRenderer();
    void draw() override;
    void setPixels(const std::vector<unsigned char>& data, size_t width, size_t height) const;
private:
    Shader shader;
    unsigned int vao, vbo, textureId, transformLoc;
};

