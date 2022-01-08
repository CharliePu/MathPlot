#pragma once
#include <queue>

#include "Plot.h"
#include "Point.h"
#include "Sample.h"

class SampleMap
{
public:
	SampleMap();
	Point& get(double x, double y);
	std::vector<std::vector<Point>>& getMap();
	void fill(const Interval& xi, const Interval& yi);
	Sample getSamplePoints(const Interval& xi, const Interval& yi);
	void construct(const Plot& plot, double xStep, double yStep);

private:
	std::unique_ptr<Expression> expression;
	double xmin, ymin;
	double xstep, ystep;
	std::vector<std::vector<Point>> map;
};

