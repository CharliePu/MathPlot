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
#include <iostream>

int main()
{
    Program program(getProgram());

    RegionRenderer regionRenderer;
    LineRenderer lineRenderer;
    
    Plot plot(StatementParser().parse("y=x*x*x*x*x-4*x*x*x*x+8*x*x-0.4*x*x+1").value(), -10.0, 10.0, -8.0, 8.0);

    Rasterizer rasterizer;
    rasterizer.rasterize(plot, 0.05);

    regionRenderer.updateData(rasterizer.generateRegions(), rasterizer.getRegionWidth(), rasterizer.getRegionHeight());
    lineRenderer.updateData(rasterizer.generateLines());

    while (!program.shouldClose())
    {
        regionRenderer.draw();
        lineRenderer.draw();
    }

    return 0;
}