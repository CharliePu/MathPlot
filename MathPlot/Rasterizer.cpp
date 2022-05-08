#include "Rasterizer.h"

#include <array>
#include <algorithm>
#include <iostream>
#include <queue>

Rasterizer::Rasterizer(): thread(&Rasterizer::rasterizeTask, this), xStep(1.0), yStep(1.0)
{
}

Rasterizer::~Rasterizer()
{
    threadShouldClose = true;
    thread.join();
}

void Rasterizer::rasterizeTask()
{
    while (!threadShouldClose)
    {
        if (requestReady && !dataReady)
        {
            requestReady = false;

            plot = requestPlot;
            width = requestWidth;
            height = requestHeight;
            xStep = plot.getWidth() / static_cast<double>(width);
            yStep = plot.getHeight() / static_cast<double>(height);

            rasterize();
            if (!requestReady)
            {
                dataReady = true;
            }
        }
    }
}

void Rasterizer::requestRasterize(Plot plot, int width, int height)
{
    if (plot.empty() || width <= 0 || height <= 0)
    {
        return;
    }

    requestPlot = plot;
    requestWidth = width;
    requestHeight = height;
    requestReady = true;
}

void Rasterizer::rasterize()
{
    sampleTree = SampleTree();

    samples.clear();

    std::queue<Interval> xiQueue, yiQueue;
    std::queue<std::unique_ptr<SampleTreeNode>*> nodeQueue;

    auto expression = plot.getStatement().getExpression();
    auto certaintyCheck = plot.getStatement().getIntervalCertaintyChecker();
    auto comparator = plot.getStatement().getComparator();

    xiQueue.push(Interval(plot.getXMin(), plot.getXMax()));
    yiQueue.push(Interval(plot.getYMin(), plot.getYMax()));

    nodeQueue.push(&sampleTree.root);

    while (!nodeQueue.empty())
    {
        auto& node = *nodeQueue.front();
        auto& xi = xiQueue.front();
        auto& yi = yiQueue.front();

        node = std::make_unique<SampleTreeNode>();
        node->xi = xi;
        node->yi = yi;

        bool xLimReached = (xi.upper() - xi.lower()) <= xStep;
        bool yLimReached = (yi.upper() - yi.lower()) <= yStep;

        double midX = (xi.lower() + xi.upper()) / 2.0;
        double midY = (yi.lower() + yi.upper()) / 2.0;
        auto b = expression->evaluateInterval(xi, yi);
        auto b1 = certaintyCheck(b);
        if (empty(b) || b1 || (xLimReached && yLimReached))
        {
            node->determinedState = comparator(expression->evaluate(midX, midY), 0);
            samples.push_back(node->sample);
        }
        else
        {
            if (xLimReached)
            {
                node->xSplit = false;
                node->ySplit = true;

                node->nodes.push_back(nullptr);
                xiQueue.push(xi);
                yiQueue.emplace(yi.lower(), midY);

                node->nodes.push_back(nullptr);
                xiQueue.push(xi);
                yiQueue.emplace(midY, yi.upper());

                nodeQueue.push(&node->nodes[0]);
                nodeQueue.push(&node->nodes[1]);
            }
            else if (yLimReached)
            {
                node->xSplit = true;
                node->ySplit = false;

                node->nodes.push_back(nullptr);
                xiQueue.emplace(xi.lower(), midX);
                yiQueue.push(yi);

                node->nodes.push_back(nullptr);
                xiQueue.emplace(midX, xi.upper());
                yiQueue.push(yi);

                nodeQueue.push(&node->nodes[0]);
                nodeQueue.push(&node->nodes[1]);
            }
            else
            {
                node->xSplit = true;
                node->ySplit = true;

                node->nodes.push_back(nullptr);
                xiQueue.emplace(xi.lower(), midX);
                yiQueue.emplace(yi.lower(), midY);

                node->nodes.push_back(nullptr);
                xiQueue.emplace(midX, xi.upper());
                yiQueue.emplace(yi.lower(), midY);

                node->nodes.push_back(nullptr);
                xiQueue.emplace(xi.lower(), midX);
                yiQueue.emplace(midY, yi.upper());

                node->nodes.push_back(nullptr);
                xiQueue.emplace(midX, xi.upper());
                yiQueue.emplace(midY, yi.upper());

                nodeQueue.push(&node->nodes[0]);
                nodeQueue.push(&node->nodes[1]);
                nodeQueue.push(&node->nodes[2]);
                nodeQueue.push(&node->nodes[3]);
            }
        }

        nodeQueue.pop();
        xiQueue.pop();
        yiQueue.pop();
    }

    generateRegions();
}

bool Rasterizer::isDataReady()
{
    return dataReady;
}

std::vector<unsigned char> Rasterizer::getData()
{
    dataReady = false;
    return regionData;
}

double Rasterizer::normalize(double val, double min, double max)
{
    return (val - min) / (max - min) * 2.0 - 1.0;
}

void Rasterizer::generateRegions()
{
    auto comparator = plot.getStatement().getComparator();

    std::vector<unsigned char> pixels;
    pixels.reserve((height - 1) * (width - 1));
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            pixels.push_back(sampleTree.check(plot.getXMin() + (x + 0.5) * xStep, plot.getYMin() + (y + 0.5) * yStep) ? 255 : 0);
            pixels.push_back(0);
            pixels.push_back(0);
        }

        // 4-byte alignment
        while (pixels.size() % 4 != 0)
        {
            pixels.push_back(0);
        }
    }
	
    regionData = pixels;
}

size_t Rasterizer::getWidth()
{
    return width;
}

size_t Rasterizer::getHeight()
{
    return height;
}
