#pragma once
#include <memory>
#include <optional>

#include "Statement.h"

class Plot
{
public:
	Plot();
	Plot(const Statement& statement, double xmin, double xmax, double ymin, double ymax);
	Statement getStatement() const;

	void setStatement(const Statement& statement);
	void setTargetXRange(double xMin, double xMax);
	void setTargetYRange(double yMin, double yMax);
	void setAspectRatio(double ratio);

	bool empty() const;

	double getXMin() const;
	double getXMax() const;
	double getYMin() const;
	double getYMax() const;

	double getWidth() const;
	double getHeight() const;

	void move(double x, double y);
	void zoom(double cx, double cy, double s);

private:
	std::optional<Statement> statement;
	double xmin, xmax, ymin, ymax, ratio;
};

