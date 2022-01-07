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
#include "SampleTree.h"
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
            if (width > 0 && height > 0)
            {
                plot.setAspectRatio(width / static_cast<double>(height));
                rasterizer.requestRasterize(plot, width, height);
            }
        }
        });

    while (!program.shouldClose())
    {
        if (rasterizer.isDataReady())
        {
            lineRenderer.resetTransform();
            regionRenderer.resetTransform();
            regionRenderer.updateData(rasterizer.getRegions(), rasterizer.getRegionWidth(), rasterizer.getRegionHeight());
            lineRenderer.updateData(rasterizer.getLines());
            rasterizer.closeData();
        }

        regionRenderer.draw();
        lineRenderer.draw();

        if (dataReady)
        {
            currentStatement = StatementParser().parse(s);
            if (currentStatement.has_value())
            {
                plot.setStatement(*currentStatement);

                rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());
            }
            dataReady = false;
        }

        if (program.keyPressed('I'))
        {
            inputStatus = true;
        }

        if (program.mouseDragged())
        {
            plot.move(program.getMouseDeltaX() * (plot.getXMax() - plot.getXMin()), program.getMouseDeltaY() * (plot.getYMax() - plot.getYMin()));

            regionRenderer.move(program.getMouseDeltaX() * 2, -program.getMouseDeltaY() * 2);
            lineRenderer.move(program.getMouseDeltaX() * 2, -program.getMouseDeltaY() * 2);

            rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());
        }
        if (program.mouseScrolled())
        {
            plot.zoom(plot.getXMin() + program.getMouseX() * (plot.getXMax() - plot.getXMin()),
                plot.getYMax() - program.getMouseY() * (plot.getYMax() - plot.getYMin()),
                -program.getMouseScroll());

            regionRenderer.zoom(program.getMouseX() * 2 - 1, -program.getMouseY() * 2 + 1, program.getMouseScroll());
            lineRenderer.zoom(program.getMouseX() * 2 - 1, -program.getMouseY() * 2 + 1, program.getMouseScroll());
            rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());
        }
    }

    threadShouldClose = true;
    inputThread.join();

    return 0;
}