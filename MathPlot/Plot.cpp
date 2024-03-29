#include "Plot.h"
#include <iostream>

#include <stdexcept>

#include <glm/ext/matrix_transform.hpp>

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

void Plot::setStatement(const std::optional<Statement>& statement)
{
	this->statement = statement;
}

void Plot::setTargetXRange(double xMin, double xMax)
{
	xmin = xMin;
	xmax = xMax;
	const auto deltaY = ((xmax - xmin) / ratio - (ymax - ymin)) / 2.0;
	ymin -= deltaY;
	ymax += deltaY;
}

void Plot::setTargetYRange(double yMin, double yMax)
{
	ymin = yMin;
	ymax = yMax;
	const auto deltaX = ((ymax - ymin) * ratio - (xmax - xmin)) / 2.0;
	xmin -= deltaX;
	xmax += deltaX;
}

void Plot::setAspectRatio(double ratio)
{
	this->ratio = ratio;
	setTargetXRange(xmin, xmax);
}

glm::mat4 Plot::getTransformMatrix() const
{
	auto scaleMatrix = glm::scale(
		glm::mat4(1.0),
		glm::vec3(
			2.0 / (xmax - xmin),
			2.0 / (ymax - ymin),
			1.0
		));

	auto translateMatrix = glm::translate(
		glm::mat4(1.0),
		glm::vec3(
			-(xmax + xmin) / 2.0,
			-(ymax + ymin) / 2.0,
			0.0
		)
	);

	return scaleMatrix * translateMatrix;
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
	ymin -= y;
	ymax -= y;
}

void Plot::zoom(double cx, double cy, double s)
{
	auto mapNumericRange = [](const double val, const double min1, const double max1, const double min2, const double max2)
	{
		return (val - min1) / (max1 - min1) * (max2 - min2) + min2;
	};


	const double x = mapNumericRange(cx, -1, 1, getXMin(), getXMax());
	const double y = mapNumericRange(cy, -1, 1, getYMin(), getYMax());

	xmin = (xmin - x) / s + x;
	xmax = (xmax - x) / s + x;
	ymin = (ymin - y) / s + y;
	ymax = (ymax - y) / s + y;
}
