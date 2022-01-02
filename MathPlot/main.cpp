#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Program.h"
#include "RegionRenderer.h"
#include "LineRenderer.h"
#include "Expression.h"
#include "Subtract.h"
#include "ExpressionParser.h"
#include "StatementParser.h"
#include "Rasterizer.h"

int main()
{
    Program program(getProgram());

    RegionRenderer regionRenderer;
    LineRenderer lineRenderer;

    Plot plot(StatementParser().parse("y < x * x").value(), -5.0, 5.0, -2.0, 25.0);

    Rasterizer rasterizer;
    rasterizer.rasterize(plot, 0.02);

    regionRenderer.updateData(rasterizer.generateRegions(), rasterizer.getRegionWidth(), rasterizer.getRegionHeight());
    lineRenderer.updateData(rasterizer.generateLines());

    while (!program.shouldClose())
    {
        regionRenderer.draw();
        lineRenderer.draw();
    }

    return 0;
}