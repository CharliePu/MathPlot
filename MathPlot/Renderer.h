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
	Renderer(int width, int height);


	void draw();
	void updateData(const std::vector<std::vector<double>>& data, bool ineqaulity);
private:
	void updateRegionData(const std::vector<std::vector<double>>& data);
	void updateLineData(const std::vector<std::vector<double>>& data);

	void drawRegions();
	void drawLines();

	void processRect(const std::array<Point, 4>& points);
	void identifyLineSegment(std::array<Point, 3> points, std::vector<double>& vertices);
	Point findZeroPoint(Point p1, Point p2);

	bool shouldDrawRegions;

	Shader regionShader, lineShader;
	unsigned int quadVao, quadVbo, lineVao, lineVbo;
	unsigned int textureId;
};

