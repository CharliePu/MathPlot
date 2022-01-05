#include "Plot.h"

#include <stdexcept>

Plot::Plot() : statement(), xmin(-10.0), xmax(10.0), ymin(-10.0), ymax(10.0), ratio(1.0),
offsetX(0), offsetY(0), scale(1.0)
{

}

Plot::Plot(const Statement& statement, double xmin, double xmax, double ymin, double ymax):
	statement(statement), xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), ratio(1.0)
{
	if (xmin >= xmax || ymin >= ymax)
	{
		throw std::invalid_argument("xmin>=xmax, or ymin>=ymax");
	}
}

Statement Plot::getStatement()
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

bool Plot::empty()
{
	return !statement.has_value();
}

double Plot::getXMin()
{
	return xmin * scale + offsetX;
}

double Plot::getXMax()
{
	return xmax * scale + offsetX;
}

double Plot::getYMin()
{
	return ymin * scale + offsetY;
}

double Plot::getYMax()
{
	return ymax * scale + offsetY;
}

void Plot::move(double x, double y)
{
	offsetX -= scale * x;
	offsetY += scale * y;
}

void Plot::zoom(double s)
{
	scale *= pow(1.1, s);
}
