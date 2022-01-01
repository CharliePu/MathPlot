#include "Plot.h"

#include <stdexcept>

Plot::Plot(const Statement& statement, double xmin, double xmax, double ymin, double ymax):
	statement(statement), xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
{
	if (!statement || xmin >= xmax || ymin >= ymax)
	{
		throw std::invalid_argument("statement is nullptr, xmin>=xmax, or ymin>=ymax");
	}
}

double Plot::getXMin()
{
	return xmin;
}

double Plot::getXMax()
{
	return xmax;
}

double Plot::getYMin()
{
	return ymin;
}

double Plot::getYMax()
{
	return ymax;
}
