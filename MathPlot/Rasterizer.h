#pragma once
#include <thread>
#include "Plot.h"

#include "Point.h"
#include "SampleMap.h"
#include "SampleTree.h"

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

	void generateRegions();

	double normalize(double val, double min, double max);


	std::atomic_bool requestReady, dataReady, threadShouldClose;

	int width, height;
	double xStep, yStep;
	Plot plot;
	
	int requestWidth, requestHeight;
	Plot requestPlot;

	std::vector<std::vector<Point>> map;
	SampleTree sampleTree;

	std::thread thread;

	std::vector<unsigned char> regionData;

	std::vector<Sample> samples;
};