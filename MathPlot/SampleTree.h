#pragma once
#include <array>
#include <memory>
#include <optional>

#include "Point.h"
#include "Expression.h"
#include "Plot.h"
#include "Sample.h"
#include "Interval.h"

struct SampleTreeNode
{
	Sample sample;
	std::optional<bool> determinedState;
	std::vector<std::unique_ptr<SampleTreeNode>> nodes;
	Interval xi, yi;
	bool xSplit, ySplit;
};

struct SampleTree
{
	std::unique_ptr<SampleTreeNode> root;
	bool check(double x, double y);
private:
	bool check(std::unique_ptr<SampleTreeNode>* node, double x, double y);
};

