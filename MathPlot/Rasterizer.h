#pragma once
#include "Plot.h"

#include "Point.h"

class Rasterizer
{
public:
	void rasterize(Plot plot, double step);

	std::vector<unsigned char> generateRegions();
	size_t getRegionWidth();
	size_t getRegionHeight();


	std::vector<double> generateLines();
	void processRect(const std::array<Point, 4>& points, std::vector<double>& vertices);
	void identifyLineSegment(std::array<Point, 3> points, std::vector<double>& vertices);
	Point findZeroPoint(Point p1, Point p2);
private:
	std::vector<std::vector<Point>> data;
};