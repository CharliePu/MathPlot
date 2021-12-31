#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Program.h"
#include "Renderer.h"
#include "Expression.h"
#include "ExpressionParser.h"

int main()
{
    Program program(getProgram());
    Renderer renderer(program.screenWidth, program.screenHeight);

    std::vector<std::vector<double>> data;
    for (double y = -5.0; y <= 100.0; y+=0.1)
    {
        std::vector<double> row;
        for (double x = -10.0; x <= 10.0; x+=0.1)
        {
            row.push_back(abs(x*x-y) < 1.0 ? 1.0 : 0.0);
        }
        data.push_back(row);
    }

    renderer.updateData(data);

    while (!program.shouldClose())
    {
        renderer.draw();
    }

    return 0;
}