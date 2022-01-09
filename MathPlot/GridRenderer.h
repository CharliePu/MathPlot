#pragma once
#include "Renderer.h"
class Plot;

class GridRenderer :
    public Renderer
{
public:
    GridRenderer();

	void draw() override;
    void updatePlot(const Plot& plot);
private:
    unsigned int vao, vbo, transformLoc;
    Shader shader;
};

