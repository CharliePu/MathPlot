#include "Rasterizer.h"

#include <array>
#include <algorithm>

void Rasterizer::rasterize(Plot plot, double step)
{
    double xmin = plot.getXMin();
    double xmax = plot.getXMax();
    double ymin = plot.getYMin();
    double ymax = plot.getYMax();

    Statement statement = plot.getStatement();

	data.clear();
    for (double y = ymin; y <= ymax; y += step)
    {
        std::vector<Point> row;
        for (double x = xmin; x <= xmax; x += step)
        {
            row.emplace_back(x, y, static_cast<double>(statement.evaluate(x, y)));
        }
        data.emplace_back(row);
    }
}

std::vector<double> Rasterizer::generateLines()
{
	return std::vector<double>();
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

    size_t onPointIndex = -1, abovePointIndex = -1, belowPointIndex = 0;
    for (size_t i = 0; i < points.size(); i++)
    {
        if (onPointIndex != -1 && points[i].value == 0)
        {
            onPointIndex = i;
        }
        if (abovePointIndex != -1 && points[i].value > 0)
        {
            abovePointIndex = i;
            break;
        }
    }

    size_t belowCount = onPointIndex;
    size_t onCount = abovePointIndex - onPointIndex;
    size_t aboveCount = vertices.size() - abovePointIndex;

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

    if (onCount == 1 && aboveCount == 1 && belowCount == 0)
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

    vertices.push_back(p1.x);
    vertices.push_back(p1.y);
    vertices.push_back(p2.x);
    vertices.push_back(p2.y);
}

Point Rasterizer::findZeroPoint(Point p1, Point p2)
{
    Point p((p2 - p1) * (0.0 - p1.value) / (p2.value - p1.value));
    p.value = 0;
    return p;
}


std::vector<unsigned char> Rasterizer::generateRegions()
{
    std::vector<unsigned char> flattenedData;
    flattenedData.reserve(data.size() * data.front().size());
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data.front().size(); j++)
        {
            flattenedData.push_back(static_cast<unsigned char>(data[i][j].value * 255));
        }

        // 4-byte alignment
        while (flattenedData.size() % 4 != 0)
        {
            flattenedData.push_back(0);
        }
    }
	return flattenedData;
}

size_t Rasterizer::getRegionWidth()
{
    return data.front().size();
}

size_t Rasterizer::getRegionHeight()
{
    return data.size();
}
