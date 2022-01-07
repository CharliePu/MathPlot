#pragma once
#include <thread>
#include "Plot.h"

#include "Point.h"
#include "SampleMap.h"
#include "SampleTree.h"

class Rasterizer
{
	using Interval = boost::numeric::interval<double>;
public:
	Rasterizer();
	~Rasterizer();

	void requestRasterize(Plot plot, double width, double height);

	bool isDataReady();
	void closeData();

	std::vector<unsigned char> getRegions();
	std::vector<double> getLines();

	size_t getRegionWidth();
	size_t getRegionHeight();

private:
	void rasterizeTask();

	void rasterize();

	void generateLines();
	void generateRegions();

	double normalize(double val, double min, double max);
	void processRect(const std::array<Point*, 4>& points, std::vector<double>& vertices);
	void identifyLineSegment(std::array<Point*, 3> points, std::vector<double>& vertices);
	Point findZeroPoint(Point* p1, Point* p2);


	std::atomic_bool requestReady, dataReady, threadShouldClose;

	double xStep, yStep;
	Plot plot;

	double requestXStep, requestYStep;
	Plot requestPlot;

	std::vector<std::vector<Point>> map;
	SampleTree sampleTree;
	SampleMap sampleMap;

	std::thread thread;

	std::vector<unsigned char> regionData;
	std::vector<double> lineData;

	std::vector<Sample> samples;
};