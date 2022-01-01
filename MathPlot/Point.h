#pragma once
struct Point
{
	Point(double x = 0, double y = 0, double value = 0);

	Point& operator+(const Point& p);
	Point& operator-(const Point& p);
	Point& operator*(double c);
	Point& operator/(double c);
	bool operator<(const Point& p);

	double x;
	double y;
	double value;
};