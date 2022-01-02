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
    
    auto statement = StatementParser().parse("x*x*x*x*x-4*x*x*x*x+8*x*x-0.4*x*x+1=y").value();
    std::cout<<statement.getString() << std::endl;
    Plot plot(StatementParser().parse("x*x*x*x*x-4*x*x*x*x+8*x*x-0.4*x*x+1=y").value(), -20.0, 20.0, -80.0, 80.0);

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