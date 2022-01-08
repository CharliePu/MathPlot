#include "SampleMap.h"

SampleMap::SampleMap() = default;


Point& SampleMap::get(double x, double y)
{
	int newX = (x - xmin) / xstep;
	int newY = (y - ymin) / ystep;
	return map[newY][newX];
}

std::vector<std::vector<Point>>& SampleMap::getMap()
{
	return map;
}

void SampleMap::fill(const Interval& xi, const Interval& yi)
{
	double bl = expression->evaluate(xi.lower(), yi.lower());
	double br = expression->evaluate(xi.upper(), yi.lower());
	double ul = expression->evaluate(xi.lower(), yi.upper());
	double ur = expression->evaluate(xi.upper(), yi.upper());

	double value = (bl + br + ul + ur) / 4.0;

	for (double x = xi.lower(); x <= xi.upper();)
	{
		for (double y = yi.lower(); y <= yi.upper();)
		{
			// May be biased, x and y not aligned
			get(x, y) = Point(x, y, value);
			y += ystep;
		}
		x += xstep;
	}

	get(xi.lower(), yi.lower()) = Point(xi.lower(), yi.lower(), bl);
	get(xi.upper(), yi.lower()) = Point(xi.upper(), yi.lower(), br);
	get(xi.lower(), yi.upper()) = Point(xi.lower(), yi.upper(), ul);
	get(xi.upper(), yi.upper()) = Point(xi.upper(), yi.upper(), ur);

}

Sample SampleMap::getSamplePoints(const Interval& xi,
                                  const Interval& yi)
{
	Sample sample;
	sample.points = {
		&get(xi.lower(),yi.lower()),
		& get(xi.upper(),yi.lower()),
		& get(xi.lower(),yi.upper()),
		& get(xi.upper(),yi.upper())
	};

	return sample;
}

void SampleMap::construct(const Plot& plot, double xStep, double yStep)
{
	expression = plot.getStatement().getExpression();
	xmin = plot.getXMin();
	ymin = plot.getYMin();
	xstep = xStep;
	ystep = yStep;
	map = std::vector<std::vector<Point>>(plot.getHeight() / yStep + 1, std::vector<Point>(plot.getWidth() / xStep + 1));
}
