#pragma once
#include <optional>
#include <thread>
#include "Plot.h"

#include "Point.h"

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	void requestRasterize(Plot plot, double width, double height);

	bool isDataReady();
	void closeData();

	std::vector<unsigned char> getRegions();
	std::vector<double> getLines();

	void generateRegions();
	size_t getRegionWidth();
	size_t getRegionHeight();

	void generateLines();
private:
	void rasterize(Plot plot, double xStep, double yStep);

	double normalize(double val, double min, double max);
	void processRect(const std::array<Point, 4>& points, std::vector<double>& vertices);
	void identifyLineSegment(std::array<Point, 3> points, std::vector<double>& vertices);
	Point findZeroPoint(Point p1, Point p2);

	double xStep, yStep;
	Plot plot;
	std::vector<std::vector<Point>> data;

	std::thread thread;

	void rasterizeTask();
	std::atomic_bool requestReady, dataReady, threadShouldClose;

	std::vector<unsigned char> regionData;
	std::vector<double> lineData;
};