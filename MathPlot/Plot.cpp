#include "Plot.h"
#include <iostream>

#include <stdexcept>

Plot::Plot() : statement(), xmin(-10.0), xmax(10.0), ymin(-10.0), ymax(10.0), ratio(1.0)
{

}

Plot::Plot(const Statement& statement, double xmin, double xmax, double ymin, double ymax):
	statement(statement), xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), ratio(1.0)
{
	std::cout << xmax << ", " << ymax << std::endl;
	if (xmin >= xmax || ymin >= ymax)
	{
		throw std::invalid_argument("xmin>=xmax, or ymin>=ymax");
	}
}

Statement Plot::getStatement() const
{
	return statement.value();
}

void Plot::setStatement(const Statement& statement)
{
	this->statement = statement;
}

void Plot::setTargetXRange(double xMin, double xMax)
{
	xmin = xMin;
	xmax = xMax;
	auto deltaY = ((xmax - xmin) / ratio - (ymax - ymin)) / 2.0;
	ymin -= deltaY;
	ymax += deltaY;
}

void Plot::setTargetYRange(double yMin, double yMax)
{
	ymin = yMin;
	ymax = yMax;
	auto deltaX = ((ymax - ymin) * ratio - (xmax - xmin)) / 2.0;
	xmin -= deltaX;
	xmax += deltaX;
}

void Plot::setAspectRatio(double ratio)
{
	this->ratio = ratio;
	setTargetXRange(xmin, xmax);
}

bool Plot::empty() const
{
	return !statement.has_value();
}

double Plot::getXMin() const
{
	return xmin;
}

double Plot::getXMax() const
{
	return xmax;
}

double Plot::getYMin() const
{
	return ymin;
}

double Plot::getYMax() const
{
	return ymax;
}

double Plot::getWidth() const
{
	return xmax - xmin;
}

double Plot::getHeight() const
{
	return ymax - ymin;
}

void Plot::move(double x, double y)
{
	xmin -= x;
	xmax -= x;
	ymin += y;
	ymax += y;
}

void Plot::zoom(double cx, double cy, double s)
{
	double scale = pow(1.1, s);
	xmin = (xmin - cx) * scale + cx;
	xmax = (xmax - cx) * scale + cx;
	ymin = (ymin - cy) * scale + cy;
	ymax = (ymax - cy) * scale + cy;
}
