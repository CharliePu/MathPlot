#pragma once
#include <thread>
#include "Plot.h"

#include "Point.h"

using IInterval = boost::numeric::interval<int>;

struct IntervalNode
{
	IInterval xi, yi;
	bool value;
	std::unique_ptr<std::array<IntervalNode, 2>> children;
};

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	void requestRasterize(Plot plot, int width, int height);

	bool isDataReady();

	std::vector<unsigned char> getData();
	std::vector<double> getLines();

	size_t getWidth();
	size_t getHeight();

private:

	void rasterizeTask();

	void rasterize();

	bool checkPixel(int x, int y);

	double normalize(double val, double min, double max);


	std::atomic_bool requestReady, dataReady, threadShouldClose;

	int width, height;
	double xStep, yStep;
	Plot plot;

	int requestWidth, requestHeight;
	Plot requestPlot;

	std::vector<std::vector<Point>> map;

	std::unique_ptr<IntervalNode> rootNode;

	std::thread thread;

	std::vector<unsigned char> regionData;
};