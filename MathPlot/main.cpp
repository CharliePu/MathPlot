#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Program.h"
#include "Renderer.h"
#include "Expression.h"
#include "Subtract.h"
#include "ExpressionParser.h"
#include "StatementParser.h"
#include "Rasterizer.h"

int main()
{
    Program program(getProgram());
    Renderer renderer(program.screenWidth, program.screenHeight);

    Plot plot(StatementParser().parse("y < x * x - 2xy").value(), -5.0, 5.0, -2.0, 25.0);
    Rasterizer rasterizer;
    rasterizer.rasterize(plot, 0.02);
    renderer.updateData(rasterizer.generateRegions(), true);

    while (!program.shouldClose())
    {
        renderer.draw();
    }

    return 0;
}