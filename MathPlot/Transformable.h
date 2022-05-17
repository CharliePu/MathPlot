#pragma once

#include <glm/mat4x4.hpp>

class Transformable
{
public:
	Transformable();

	void move(double x, double y);
	void zoom(double x, double y, double s);
	void resetTransform();
	glm::mat4 getTransMat();
private:
	glm::mat4 transMat;
	double netScale;
};

