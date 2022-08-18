#pragma once

#include <glm/mat4x4.hpp>

class Transformable
{
public:
	Transformable();

	void move(double x, double y);
	void zoom(double centerX, double centerY, double scale);
	void resetTransform();
	[[nodiscard]] glm::mat4 getTransMat() const;
private:
	glm::mat4 transMat;
	double netScale;
};

