#pragma once
#include <thread>
#include "Plot.h"

#include "Point.h"

using IInterval = boost::numeric::interval<int>;

struct IntervalNode
{
	IInterval xi, yi;
	double value;
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

	size_t getWidth();
	size_t getHeight();

private:

	void rasterizeTask();

	void rasterize();

	double checkPixel(int x, int y);

	std::atomic_bool requestReady, dataReady, threadShouldClose;

	int width, height;
	Plot plot;

	int requestWidth, requestHeight;
	Plot requestPlot;

	std::unique_ptr<IntervalNode> rootNode;

	std::thread thread;

	std::vector<unsigned char> pixels;
};