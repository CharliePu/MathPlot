#pragma once
#include <array>
#include <memory>
#include <optional>

#include "Point.h"
#include "Expression.h"
#include "Plot.h"
#include "Sample.h"

struct SampleTreeNode
{
	Sample sample;
	std::vector<std::unique_ptr<SampleTreeNode>> nodes;
};

struct SampleTree
{
	std::unique_ptr<SampleTreeNode> root;
};

