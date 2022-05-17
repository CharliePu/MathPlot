#pragma once
#include <optional>

#include "Statement.h"

class Plot
{
public:
	Plot();
	Plot(const Statement& statement, double xmin, double xmax, double ymin, double ymax);
	[[nodiscard]] Statement getStatement() const;

	void setStatement(const std::optional<Statement>& statement);
	void setTargetXRange(double xMin, double xMax);
	void setTargetYRange(double yMin, double yMax);
	void setAspectRatio(double ratio);

	[[nodiscard]] bool empty() const;

	[[nodiscard]] double getXMin() const;
	[[nodiscard]] double getXMax() const;
	[[nodiscard]] double getYMin() const;
	[[nodiscard]] double getYMax() const;

	[[nodiscard]] double getWidth() const;
	[[nodiscard]] double getHeight() const;

	void move(double x, double y);
	void zoom(double cx, double cy, double s);

private:
	std::optional<Statement> statement;
	double xmin, xmax, ymin, ymax, ratio;
};

