#pragma once
#include <vector>

#include "Shader.h"

struct Point;

namespace MathPlotTests
{
	class RendererTest;
}

class Renderer
{
	friend class MathPlotTests::RendererTest;
public:
	virtual void draw() = 0;
	virtual void updateData(const std::vector<std::vector<double>>& data) = 0;
};

