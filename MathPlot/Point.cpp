#include "Point.h"

Point::Point(const Point& other) = default;

Point::Point(Point&& other) noexcept: x(other.x),
                                      y(other.y),
                                      value(other.value)
{
}

Point& Point::operator=(const Point& other)
{
	if (this == &other)
		return *this;
	x = other.x;
	y = other.y;
	value = other.value;
	return *this;
}

Point& Point::operator=(Point&& other) noexcept
{
	if (this == &other)
		return *this;
	x = other.x;
	y = other.y;
	value = other.value;
	return *this;
}

Point::Point(double x, double y, double value): x(x), y(y), value(value)
{
}

Point Point::operator+(const Point& p)
{
	return Point(this->x + p.x, this->y + p.y, this->value + p.value);
}

Point Point::operator-(const Point& p)
{
	return Point(this->x - p.x, this->y - p.y, this->value - p.value);
}

Point Point::operator*(double c)
{
	return Point(this->x * c, this->y * c, this->value);
}

Point Point::operator/(double c)
{
	return Point(this->x / c, this->y / c, this->value);
}

bool Point::operator<(const Point& p)
{
	return value < p.value;
}