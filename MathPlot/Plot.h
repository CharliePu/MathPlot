#pragma once
#include <memory>
#include <optional>

#include "Statement.h"

class Plot
{
public:
	Plot();
	Plot(const Statement& statement, double xmin, double xmax, double ymin, double ymax);
	Statement getStatement();

	void setStatement(const Statement& statement);
	void setTargetXRange(double xMin, double xMax);
	void setTargetYRange(double yMin, double yMax);
	void setAspectRatio(double ratio);

	bool empty();

	double getXMin();
	double getXMax();
	double getYMin();
	double getYMax();

private:
	std::optional<Statement> statement;
	double xmin, xmax, ymin, ymax, ratio;
};

