#include "Rasterizer.h"

#include <array>
#include <algorithm>

void Rasterizer::rasterize(Plot plot, double step)
{
    double xmin = plot.getXMin();
    double xmax = plot.getXMax();
    double ymin = plot.getYMin();
    double ymax = plot.getYMax();

    this->plot = plot;

    auto expression = plot.getStatement().getExpression()->clone();

	data.clear();
    for (double y = ymin; y <= ymax; y += step)
    {
        std::vector<Point> row;
        for (double x = xmin; x <= xmax; x += step)
        {
            row.emplace_back((x - xmin)/(xmax - xmin) * 2.0 - 1.0,
                (y - ymin) / (ymax - ymin) * 2.0 -1.0, 
                static_cast<double>(expression->evaluate(x, y)));
        }
        data.emplace_back(row);
    }
}

std::vector<double> Rasterizer::generateLines()
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
    
	return vertices;
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

    vertices.push_back(p1.x);
    vertices.push_back(p1.y);
    vertices.push_back(p2.x);
    vertices.push_back(p2.y);
}

Point Rasterizer::findZeroPoint(Point p1, Point p2)
{
    Point p(p1 + (p2 - p1) * (0.0 - p1.value) / (p2.value - p1.value));
    p.value = 0;
    return p;
}


std::vector<unsigned char> Rasterizer::generateRegions()
{
    if (!plot.has_value())
    {
        return std::vector<unsigned char>();
    }

    auto comparator = plot->getStatement().getComparator();

    std::vector<std::vector<unsigned char>> pixels(data.size() - 1, std::vector<unsigned char>(data.front().size() - 1));

    for (size_t y = 0; y < data.size() - 1; y++)
    {
        for (size_t x = 0; x < data.front().size() - 1; x++)
        {
            pixels[y][x] = 
                comparator(data[y][x].value, 0) &&
                comparator(data[y][x + 1].value, 0) &&
                comparator(data[y + 1][x].value, 0) &&
                comparator(data[y + 1][x + 1].value, 0);
        }
    }

    std::vector<unsigned char> flattenedPixel;
    flattenedPixel.reserve(pixels.size() * pixels.front().size());
    for (int i = 0; i < pixels.size(); i++)
    {
        for (int j = 0; j < pixels.front().size(); j++)
        {
            flattenedPixel.push_back(pixels[i][j] * 255);
        }

        // 4-byte alignment
        while (flattenedPixel.size() % 4 != 0)
        {
            flattenedPixel.push_back(0);
        }
    }
	return flattenedPixel;
}

size_t Rasterizer::getRegionWidth()
{
    return data.front().size() - 1;
}

size_t Rasterizer::getRegionHeight()
{
    return data.size() - 1;
}
