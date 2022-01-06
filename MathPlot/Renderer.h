#pragma once
#include <vector>

#include <glm/mat4x4.hpp>

#include "Shader.h"

namespace MathPlotTests
{
	class RendererTest;
}

class Renderer
{
	friend class MathPlotTests::RendererTest;
public:
	Renderer();

	virtual void draw() = 0;
	void move(double x, double y);
	void zoom(double x, double y, double s);
	void resetTransform();
protected:
	glm::mat4 getTransMat();
private:
	glm::mat4 transMat;
	double netScale;
};

