#pragma once
struct Point
{
	Point(const Point& other);

	Point(Point&& other) noexcept;

	Point& operator=(const Point& other);

	Point& operator=(Point&& other) noexcept;

	Point(double x = 0, double y = 0, double value = 0);
	

	Point operator+(const Point& p);
	Point operator-(const Point& p);
	Point operator*(double c);
	Point operator/(double c);
	bool operator<(const Point& p);

	double x;
	double y;
	double value;
};