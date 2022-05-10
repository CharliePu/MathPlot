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
#include "GridRenderer.h"
#include <iostream>
#include <atomic>
#include <mutex>
#include <condition_variable>

std::atomic<bool> threadShouldClose, dataReady;
bool inputRequested;
std::condition_variable inputRequestedCv;
std::mutex inputRequestedMutex;

std::string s;

void concurrentInput()
{
    while (!threadShouldClose)
    {
        std::unique_lock<std::mutex> inputRequestedLock(inputRequestedMutex);
        inputRequestedCv.wait(inputRequestedLock, [] {
            return (inputRequested && !dataReady);
        });
        inputRequestedLock.unlock();

        if (!threadShouldClose)
        {
            std::cout << "Input an equation/inequality: ";
            std::cin >> s;
            dataReady = true;
            inputRequestedLock.lock();
            inputRequested = false;
            inputRequestedLock.unlock();
        }
    }
}

int main()
{
    Program program(getProgram());

    Rasterizer rasterizer;

    RegionRenderer regionRenderer;
    LineRenderer lineRenderer;
    GridRenderer gridRenderer;
    
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
            regionRenderer.resetTransform();
            regionRenderer.setPixels(rasterizer.getData(), rasterizer.getWidth(), rasterizer.getHeight());
        }

        gridRenderer.draw();
        regionRenderer.draw();

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
            std::unique_lock<std::mutex> inputRequestLock(inputRequestedMutex);
            inputRequested = true;
            inputRequestLock.unlock();
            inputRequestedCv.notify_one();
        }
        if (program.keyPressed('D'))
        {
            rasterizer.toggleDebug();
        }

        if (program.mouseDragged())
        {
            plot.move(program.getMouseDeltaX() * (plot.getXMax() - plot.getXMin()), program.getMouseDeltaY() * (plot.getYMax() - plot.getYMin()));
            gridRenderer.updatePlot(plot);

            regionRenderer.move(program.getMouseDeltaX() * 2, -program.getMouseDeltaY() * 2);
            lineRenderer.move(program.getMouseDeltaX() * 2, -program.getMouseDeltaY() * 2);

            rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());
        }
        if (program.mouseScrolled())
        {
            plot.zoom(plot.getXMin() + program.getMouseX() * (plot.getXMax() - plot.getXMin()),
                plot.getYMax() - program.getMouseY() * (plot.getYMax() - plot.getYMin()),
                -program.getMouseScroll());
            gridRenderer.updatePlot(plot);

            regionRenderer.zoom(program.getMouseX() * 2 - 1, -program.getMouseY() * 2 + 1, program.getMouseScroll());
            lineRenderer.zoom(program.getMouseX() * 2 - 1, -program.getMouseY() * 2 + 1, program.getMouseScroll());
            rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());
        }
    }

    threadShouldClose = true;
    dataReady = false;
    std::unique_lock<std::mutex> inputRequestedLock(inputRequestedMutex);
    inputRequested = true;
    inputRequestedLock.unlock();
    inputRequestedCv.notify_one();
    inputThread.join();

    return 0;
}