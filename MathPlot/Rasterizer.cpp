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
            xStep = requestXStep;
            yStep = requestYStep;

            rasterize();
            if (!requestReady)
            {
                dataReady = true;
            }
        }
    }
}

void Rasterizer::requestRasterize(Plot plot, double width, double height)
{
    if (plot.empty() || width <= 0 || height <= 0)
    {
        return;
    }

    requestPlot = plot;
    requestXStep = plot.getWidth() / width;
    requestYStep = plot.getHeight() / height;
    requestReady = true;
}

void Rasterizer::rasterize()
{
    sampleMap.construct(plot, xStep, yStep);
    sampleTree = SampleTree();

    samples.clear();

    std::queue<Interval> xiQueue, yiQueue;
    std::queue<std::unique_ptr<SampleTreeNode>*> nodeQueue;

    auto expression = plot.getStatement().getExpression();
    auto certaintyCheck = plot.getStatement().getIntervalCertaintyChecker();

    xiQueue.push(Interval(plot.getXMin(), plot.getXMax()));
    yiQueue.push(Interval(plot.getYMin(), plot.getYMax()));

    nodeQueue.push(&sampleTree.root);

    while (!nodeQueue.empty())
    {
        auto& node = *nodeQueue.front();
        auto& xi = xiQueue.front();
        auto& yi = yiQueue.front();

        node = std::make_unique<SampleTreeNode>();

        bool xLimReached = (xi.upper() - xi.lower()) <= xStep;
        bool yLimReached = (yi.upper() - yi.lower()) <= yStep;

        sampleMap.fill(xi, yi);

        if (certaintyCheck(expression->evaluateInterval(xi, yi)) || (xLimReached && yLimReached))
        {
            node->sample = sampleMap.getSamplePoints(xi, yi);
            samples.push_back(node->sample);
        }
        else
        {
            double midX = (xi.lower() + xi.upper()) / 2.0;
            double midY = (yi.lower() + yi.upper()) / 2.0;
            if (xLimReached)
            {
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

    generateLines();
    generateRegions();
}

bool Rasterizer::isDataReady()
{
    return dataReady;
}

void Rasterizer::closeData()
{
    dataReady = false;
}

std::vector<unsigned char> Rasterizer::getRegions()
{
    return regionData;
}

std::vector<double> Rasterizer::getLines()
{
    return lineData;
}

void Rasterizer::generateLines()
{
    std::vector<double> vertices;
    for (auto& sample : samples)
    {
        processRect(sample.points, vertices);
    }
    //for (size_t y = data.size() - 1; y > 0; y--)
    //{
    //    for (size_t x = 0; x < data.front().size() - 1; x++)
    //    {
    //        processRect({
    //            data[y][x],
    //            data[y][x + 1],
    //            data[y - 1][x],
    //            data[y - 1][x + 1]
    //            }, vertices);
    //    }
    //}
    
    lineData = vertices;
}

double Rasterizer::normalize(double val, double min, double max)
{
    return (val - min) / (max - min) * 2.0 - 1.0;
}

void Rasterizer::processRect(const std::array<Point*, 4>& points, std::vector<double>& vertices)
{
    double average = 0;
    for (auto& point : points)
    {
        average += point->value;
    }
    average /= points.size();

    Point center(
        (points[0]->x + points[1]->x) / 2.0,
        (points[0]->y + points[2]->y) / 2.0,
        average);

    identifyLineSegment({ points[0], points[1], &center }, vertices);
    identifyLineSegment({ &center, points[1], points[3] }, vertices);
    identifyLineSegment({ &center, points[3], points[2] }, vertices);
    identifyLineSegment({ points[0], &center, points[2] }, vertices);
}

void Rasterizer::identifyLineSegment(std::array<Point*, 3> points, std::vector<double>& vertices)
{
    std::sort(points.begin(), points.end(), [](Point *ptr1, Point *ptr2)
    {
            return ptr1->value < ptr2->value;
    });


    size_t onPointIndex = 0, abovePointIndex = 0, belowPointIndex = 0;
    size_t belowCount = 0, onCount = 0, aboveCount = 0;

    int i = 0;
    while (i < points.size() && points[i]->value < 0)
    {
        i++;
        belowCount++;
    }

    onPointIndex = i;
    while (i < points.size() && points[i]->value == 0)
    {
        i++;
        onCount++;
    }

    abovePointIndex = i;
    aboveCount = points.size() - belowCount - onCount;

    if (aboveCount == 3 || belowCount == 3 || onCount == 3)
    {
        return;
    }

    if (onCount == 1 && (aboveCount == 2 || belowCount == 2))
    {
        return;
    }

    Point p1, p2;

    if (onCount == 2)
    {
        p1 = *points[onPointIndex];
        p2 = *points[onPointIndex + 1];
    }

    if (onCount == 1 && aboveCount == 1 && belowCount == 1)
    {
        p1 = findZeroPoint(points[abovePointIndex], points[belowPointIndex]);
        p2 = *points[onPointIndex];
    }

    if (onCount == 0)
    {
        if (belowCount == 2)
        {
            p1 = findZeroPoint(points[abovePointIndex], points[belowPointIndex]);
            p2 = findZeroPoint(points[abovePointIndex], points[belowPointIndex + 1]);

        }
        if (aboveCount == 2)
        {
            p1 = findZeroPoint(points[belowPointIndex], points[abovePointIndex]);
            p2 = findZeroPoint(points[belowPointIndex], points[abovePointIndex + 1]);
        }
    }

    vertices.push_back(normalize(p1.x, plot.getXMin(), plot.getXMax()));
    vertices.push_back(normalize(p1.y, plot.getYMin(), plot.getYMax()));
    vertices.push_back(normalize(p2.x, plot.getXMin(), plot.getXMax()));
    vertices.push_back(normalize(p2.y, plot.getYMin(), plot.getYMax()));
}

Point Rasterizer::findZeroPoint(Point *p1, Point *p2)
{
    Point p(*p1 + (*p2 - *p1) * (0.0 - p1->value) / (p2->value - p1->value));
    p.value = 0;
    return p;
}


void Rasterizer::generateRegions()
{
    auto comparator = plot.getStatement().getComparator();
    auto map = sampleMap.getMap();

    std::vector<unsigned char> pixels;
    pixels.reserve((map.size() - 1) * (map.front().size() - 1));
    for (int i = 0; i < map.size() - 1; i++)
    {
        for (int j = 0; j < map.front().size() - 1; j++)
        {
            auto valid = 
                comparator(map[i][j].value, 0) &&
                comparator(map[i][j + 1].value, 0) &&
                comparator(map[i + 1][j].value, 0) &&
                comparator(map[i + 1][j + 1].value, 0);
            pixels.push_back(valid ? 255 : 0);
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

size_t Rasterizer::getRegionWidth()
{
    return sampleMap.getMap().front().size() - 1;
}

size_t Rasterizer::getRegionHeight()
{
    return sampleMap.getMap().size() - 1;
}
