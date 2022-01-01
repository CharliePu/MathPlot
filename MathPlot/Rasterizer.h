#pragma once
#include "Plot.h"

class Rasterizer
{
public:
	void rasterize(Plot plot, double step);
	std::vector<std::vector<double>> generateLines();
	std::vector<std::vector<double>> generateRegions();
private:
	std::vector<std::vector<double>> data;
};

