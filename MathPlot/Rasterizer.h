#pragma once
#include <thread>
#include <condition_variable>
#include "Plot.h"

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

	void toggleDebug();

	size_t getWidth();
	size_t getHeight();

private:

	void rasterizeTask();

	void rasterize();

	double getPixel(int x, int y);

	bool checkDebugFrame(int x, int y);

	std::atomic_bool threadShouldClose;
	bool requestReady, dataReady;

	std::mutex requestReadyMutex, dataReadyMutex;
	std::condition_variable requestReadyCv;

	bool debugEnabled;

	int width, height;
	Plot plot;

	int requestWidth, requestHeight;
	Plot requestPlot;

	std::unique_ptr<IntervalNode> rootNode;

	std::thread thread;

	std::vector<unsigned char> pixels;
};