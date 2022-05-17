#pragma once
#include "Drawable.h"
#include "Shader.h"
#include "Transformable.h"

class Plot;

class GridRenderer :
    public Drawable
{
public:
    GridRenderer();

	void draw() override;
    void updatePlot(const Plot& plot);
private:
    unsigned int vao, vbo;
    Shader shader;
};

