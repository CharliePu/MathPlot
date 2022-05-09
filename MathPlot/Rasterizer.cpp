#include "Rasterizer.h"

#include <array>
#include <algorithm>
#include <iostream>
#include <queue>

Rasterizer::Rasterizer(): thread(&Rasterizer::rasterizeTask, this)
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
    auto getPixelInterval = [&](Interval interval, double width) {
        double lowerBound = interval.lower() / width;
        double upperBound = (interval.upper() + 1) / width;

        return Interval(lowerBound, upperBound);
    };

    auto mapToInterval = [&](Interval normalizedInterval, Interval mappedRange) {
        double scale = boost::numeric::width(mappedRange);
        double offset = mappedRange.lower();

        return Interval(normalizedInterval.lower() * scale + offset, normalizedInterval.upper() * scale + offset);
    };


    auto expression = plot.getStatement().getExpression();
    auto certaintyCheck = plot.getStatement().getIntervalCertaintyChecker();
    auto comparator = plot.getStatement().getComparator();

    std::queue<IntervalNode*> nodeQueue;

    rootNode = std::make_unique<IntervalNode>();
    rootNode->xi = IInterval(0, width - 1);
    rootNode->yi = IInterval(0, height - 1);
    nodeQueue.push(rootNode.get());

    while (!nodeQueue.empty())
    {
        auto node = nodeQueue.front();
        nodeQueue.pop();

        Interval mappedXi = mapToInterval(getPixelInterval(node->xi, width), Interval(plot.getXMin(), plot.getXMax()));
        Interval mappedYi = mapToInterval(getPixelInterval(node->yi, height), Interval(plot.getYMin(), plot.getYMax()));

        auto evaluatedInterval = expression->evaluateInterval(mappedXi, mappedYi);

        //node->value = comparator(boost::numeric::median(evaluatedInterval), 0.0);


        // Blending lower bound and upper bound values
        double lbVal = comparator(evaluatedInterval.lower(), 0.0);
        double lbRat = abs(evaluatedInterval.lower()) / (abs(evaluatedInterval.lower()) + abs(evaluatedInterval.upper()));
        double ubVal = comparator(evaluatedInterval.upper(), 0.0);
        double ubRat = abs(evaluatedInterval.upper()) / (abs(evaluatedInterval.lower()) + abs(evaluatedInterval.upper()));
        node->value = lbVal * lbRat + ubVal * ubRat;

        if (certaintyCheck(evaluatedInterval)) {
            continue;
        }

        if (boost::numeric::width(node->xi) < 1.0 && boost::numeric::width(node->yi) < 1.0) {
            continue;
        }

        node->children = std::make_unique<std::array<IntervalNode, 2>>();

        if (boost::numeric::width(node->xi) >= 1.0 && 
            (boost::numeric::width(node->yi) <= boost::numeric::width(node->xi) || 
             boost::numeric::width(node->yi) < 1.0))
        {
            (*node->children)[0].xi = IInterval(node->xi.lower(), boost::numeric::median(node->xi));
            (*node->children)[0].yi = node->yi;

            (*node->children)[1].xi = IInterval(boost::numeric::median(node->xi) + 1, node->xi.upper());
            (*node->children)[1].yi = node->yi;
        }
        else if (boost::numeric::width(node->yi) >= 1.0)
        {
            (*node->children)[0].xi = node->xi;
            (*node->children)[0].yi = IInterval(node->yi.lower(), boost::numeric::median(node->yi));

            (*node->children)[1].xi = node->xi;
            (*node->children)[1].yi = IInterval(boost::numeric::median(node->yi) + 1, node->yi.upper());
        }

        nodeQueue.push(&(*node->children)[0]);
        nodeQueue.push(&(*node->children)[1]);
    }

    pixels.clear();
    pixels.reserve(height * width);
    for (int y = 0; y < height; y++)
    {
        //dMap.emplace_back();
        for (int x = 0; x < width; x++)
        {
            pixels.push_back(checkPixel(x, y) * 255);
            pixels.push_back(0);
            pixels.push_back(0);
        }

        // 4-byte alignment
        while (pixels.size() % 4 != 0)
        {
            pixels.push_back(0);
        }
    }
}

double Rasterizer::checkPixel(int x, int y)
{
    auto currNode = rootNode.get();

    while (currNode) {
        if (currNode->children) {
            for (auto &child : *currNode->children) {
                if (child.xi.lower() <= x && child.xi.upper() >= x &&
                    child.yi.lower() <= y && child.yi.upper() >= y) {
                    currNode = &child;
                    continue;
                }
            }
        }
        else {
            return currNode->value;
        }
    }

    return false;
}

bool Rasterizer::isDataReady()
{
    return dataReady;
}

std::vector<unsigned char> Rasterizer::getData()
{
    dataReady = false;
    return pixels;
}

size_t Rasterizer::getWidth()
{
    return width;
}

size_t Rasterizer::getHeight()
{
    return height;
}
