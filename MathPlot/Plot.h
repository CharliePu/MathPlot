#pragma once
#include <memory>

#include "Statement.h"

class Plot
{
public:
	Plot(const Statement& statement, double xmin, double xmax, double ymin, double ymax);
	double getXMin();
	double getXMax();
	double getYMin();
	double getYMax();
private:
	Statement statement;
	double xmin, xmax, ymin, ymax;
};

