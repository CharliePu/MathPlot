#pragma once
#include "Drawable.h"
#include "Shader.h"

class Plot;

class GridRenderer :
    public Drawable
{
public:
    GridRenderer();

	void draw() override;
    void updatePlot(const Plot& plot) const;
private:
    unsigned int vao, vbo;
    Shader shader;
};

