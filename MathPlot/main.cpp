#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>

#include "Program.h"
#include "RegionRenderer.h"
#include "LineRenderer.h"
#include "Expression.h"
#include "Subtract.h"
#include "ExpressionParser.h"
#include "StatementParser.h"
#include "Rasterizer.h"
#include <iostream>
#include <atomic>

std::atomic<bool> inputShouldClose, dataReady;
std::string s;

void concurrentInput()
{
    while (!inputShouldClose)
    {
        if (!dataReady)
        {
            std::cout << "Input an equation/inequality: ";
            std::cin >> s;
            dataReady = true;
        }
    }
}

int main()
{
    Program program(getProgram());

    RegionRenderer regionRenderer;
    LineRenderer lineRenderer;
    
    inputShouldClose = false;
    std::thread inputThread(concurrentInput);

    while (!program.shouldClose())
    {
        regionRenderer.draw();
        lineRenderer.draw();

        if (dataReady)
        {
            std::optional<Statement> statement = StatementParser().parse(s);
            if (statement.has_value())
            {
                Plot plot(*statement, -10.0, 10.0, -8.0, 8.0);

                Rasterizer rasterizer;
                rasterizer.rasterize(plot, 0.1);

                regionRenderer.updateData(rasterizer.generateRegions(), rasterizer.getRegionWidth(), rasterizer.getRegionHeight());
                lineRenderer.updateData(rasterizer.generateLines());
            }

            dataReady = false;
        }
    }

    inputThread.join();

    return 0;
}