#include "Point.h"

Point::Point(double x, double y, double value): x(x), y(y), value(value)
{
}

Point& Point::operator+(const Point& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

Point& Point::operator-(const Point& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

Point& Point::operator*(double c)
{
	x *= c;
	y *= c;
	return *this;
}

Point& Point::operator/(double c)
{
	x /= c;
	y /= c;
	return *this;
}

bool Point::operator<(const Point& p)
{
	return value < p.value;
}