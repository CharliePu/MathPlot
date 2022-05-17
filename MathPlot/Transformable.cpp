#include "Transformable.h"
#include <glm/ext/matrix_transform.hpp>

Transformable::Transformable() : transMat(1.0), netScale(1.0)
{
}

void Transformable::move(double x, double y)
{
	transMat = glm::translate(transMat, glm::vec3(x / netScale, y / netScale, 0));
}

void Transformable::zoom(double x, double y, double s)
{
	const double scale = pow(1.1, s);
	netScale *= scale;
	transMat = glm::translate(glm::scale(glm::translate(transMat, glm::vec3(x, y, 0)), glm::vec3(scale, scale, 1)), glm::vec3(-x, -y, 0));
}

void Transformable::resetTransform()
{
	transMat = glm::identity<glm::mat4>();
	netScale = 1.0;
}

glm::mat4 Transformable::getTransMat() const
{
	return transMat;
}

