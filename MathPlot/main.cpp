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

std::atomic<bool> threadShouldClose, dataReady, inputStatus;
std::string s;

void concurrentInput()
{
    while (!threadShouldClose)
    {
        if (inputStatus)
        {
            std::cout << "Input an equation/inequality: ";
            std::cin >> s;
            dataReady = true;
            inputStatus = false;
        }
    }
}

int main()
{
    Program program(getProgram());

    Rasterizer rasterizer;

    RegionRenderer regionRenderer;
    LineRenderer lineRenderer;
    
    std::optional<Statement> currentStatement;
    Plot plot;

    
    threadShouldClose = false;
    std::thread inputThread(concurrentInput);

    program.setOnWindowSizeChange([&](int width, int height) {
        if (!plot.empty())
        {
            plot.setAspectRatio(width / static_cast<double>(height));
            rasterizer.rasterize(plot, 0.1);

            regionRenderer.updateData(rasterizer.generateRegions(), rasterizer.getRegionWidth(), rasterizer.getRegionHeight());
            lineRenderer.updateData(rasterizer.generateLines());
        }
        });

    while (!program.shouldClose())
    {
        regionRenderer.draw();
        lineRenderer.draw();

        if (dataReady)
        {
            currentStatement = StatementParser().parse(s);
            if (currentStatement.has_value())
            {
                plot.setStatement(*currentStatement);

                rasterizer.rasterize(plot, 1.0);

                regionRenderer.updateData(rasterizer.generateRegions(), rasterizer.getRegionWidth(), rasterizer.getRegionHeight());
                lineRenderer.updateData(rasterizer.generateLines());
            }
            dataReady = false;
        }

        if (program.keyPressed('I'))
        {
            inputStatus = true;
        }

        if (program.mouseDragged())
        {
            double deltaX = program.getMouseDeltaX();
            double deltaY = program.getMouseDeltaY();
            plot.setTargetXRange(plot.getXMin() - deltaX * 0.03, plot.getXMax() - deltaX * 0.03);
            plot.setTargetYRange(plot.getYMin() + deltaY * 0.03, plot.getYMax() + deltaY * 0.03);

            rasterizer.rasterize(plot, 0.1);

            regionRenderer.updateData(rasterizer.generateRegions(), rasterizer.getRegionWidth(), rasterizer.getRegionHeight());
            lineRenderer.updateData(rasterizer.generateLines());
        }
    }

    threadShouldClose = true;
    inputThread.join();

    return 0;
}