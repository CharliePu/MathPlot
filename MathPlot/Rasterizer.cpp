#include "Rasterizer.h"

#include <array>
#include <algorithm>
#include <iostream>

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
            rasterize(plot, xStep, yStep);
            dataReady = true;
        }
    }
}

void Rasterizer::requestRasterize(Plot plot, double width, double height)
{
    if (plot.empty() || width <= 0 || height <= 0)
    {
        return;
    }

    this->plot = plot;
    this->xStep = (plot.getXMax() - plot.getXMin()) / width;
    this->yStep = (plot.getYMax() - plot.getYMin()) / height;
    requestReady = true;
}

void Rasterizer::rasterize(Plot plot, double xStep, double yStep)
{
    double xmin = plot.getXMin();
    double xmax = plot.getXMax();
    double ymin = plot.getYMin();
    double ymax = plot.getYMax();

    auto expression = plot.getStatement().getExpression()->clone();

	data.clear();
    for (double y = ymin; y <= ymax; y += yStep)
    {
        std::vector<Point> row;
        for (double x = xmin; x <= xmax; x += xStep)
        {
            row.emplace_back(x, y, 
                static_cast<double>(expression->evaluate(x, y)));
        }
        data.emplace_back(row);
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
    for (size_t y = data.size() - 1; y > 0; y--)
    {
        for (size_t x = 0; x < data.front().size() - 1; x++)
        {
            processRect({
                data[y][x],
                data[y][x + 1],
                data[y - 1][x],
                data[y - 1][x + 1]
                }, vertices);
        }
    }
    
    lineData = vertices;
}

double Rasterizer::normalize(double val, double min, double max)
{
    return (val - min) / (max - min) * 2.0 - 1.0;
}

void Rasterizer::processRect(const std::array<Point, 4>& points, std::vector<double>& vertices)
{
    double average = 0;
    for (auto& point : points)
    {
        average += point.value;
    }
    average /= points.size();

    Point center(
        (points[0].x + points[1].x) / 2.0,
        (points[0].y + points[2].y) / 2.0,
        average);

    identifyLineSegment({ points[0], points[1], center }, vertices);
    identifyLineSegment({ center, points[1], points[3] }, vertices);
    identifyLineSegment({ center, points[3], points[2] }, vertices);
    identifyLineSegment({ points[0], center, points[2] }, vertices);
}

void Rasterizer::identifyLineSegment(std::array<Point, 3> points, std::vector<double>& vertices)
{
    std::sort(points.begin(), points.end());


    size_t onPointIndex = 0, abovePointIndex = 0, belowPointIndex = 0;
    size_t belowCount = 0, onCount = 0, aboveCount = 0;

    int i = 0;
    while (i < points.size() && points[i].value < 0)
    {
        i++;
        belowCount++;
    }

    onPointIndex = i;
    while (i < points.size() && points[i].value == 0)
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
        p1 = points[onPointIndex];
        p2 = points[onPointIndex + 1];
    }

    if (onCount == 1 && aboveCount == 1 && belowCount == 1)
    {
        p1 = findZeroPoint(points[abovePointIndex], points[belowPointIndex]);
        p2 = points[onPointIndex];
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

Point Rasterizer::findZeroPoint(Point p1, Point p2)
{
    Point p(p1 + (p2 - p1) * (0.0 - p1.value) / (p2.value - p1.value));
    p.value = 0;
    return p;
}


void Rasterizer::generateRegions()
{
    auto comparator = plot.getStatement().getComparator();

    std::vector<std::vector<int>> pixels(data.size() - 1, std::vector<int>(data.front().size() - 1));

    for (size_t y = 0; y < data.size() - 1; y++)
    {
        for (size_t x = 0; x < data.front().size() - 1; x++)
        {
            //auto middleX = (data[y][x + 1].x + data[y][x].x) / 2.0, middleY = (data[y + 1][x].y + data[y][x].y) / 2.0;
            //auto middle = plot->getStatement().getExpression()->evaluate(middleX, middleY);
            //auto div = (data[y][x + 1].value + data[y][x].value - 2 * middle) / ((middleX - data[y][x].x) * (middleX - data[y][x].x))
            //    + (data[y + 1][x].value + data[y][x].value - 2 * middle) / ((middleY - data[y][x].y) * (middleY - data[y][x].y));

            //auto average = (data[y][x].value + data[y][x + 1].value + data[y + 1][x + 1].value + data[y + 1][x].value) / 4.0;
            //auto averageDiff = (average - middle) * 256;

            //auto max = std::max(data[y][x].value, std::max(data[y][x + 1].value, std::max(data[y + 1][x + 1].value, data[y + 1][x + 1].value)));
            //auto min = std::min(data[y][x].value, std::min(data[y][x + 1].value, std::min(data[y + 1][x + 1].value, data[y + 1][x + 1].value)));
            //
            //auto abnormal = middle >= max ? middle - max : (middle <= min ? min - middle : 0) * 255;
            //pixels[y][x] = std::min(255.0, std::max(-255.0, abnormal));
            pixels[y][x] = (comparator(data[y][x].value, 0) && comparator(data[y + 1][x + 1].value, 0)) * 64;
        }
    }

    std::vector<unsigned char> flattenedPixel;
    flattenedPixel.reserve(pixels.size() * pixels.front().size());
    for (int i = 0; i < pixels.size(); i++)
    {
        for (int j = 0; j < pixels.front().size(); j++)
        {
            flattenedPixel.push_back(pixels[i][j] > 0 ? pixels[i][j] : 0);
            flattenedPixel.push_back(pixels[i][j] < 0 ? -pixels[i][j] : 0);
            flattenedPixel.push_back(0);
        }

        // 4-byte alignment
        while (flattenedPixel.size() % 4 != 0)
        {
            flattenedPixel.push_back(0);
        }
    }
	
    regionData = flattenedPixel;
}

size_t Rasterizer::getRegionWidth()
{
    return data.front().size() - 1;
}

size_t Rasterizer::getRegionHeight()
{
    return data.size() - 1;
}
