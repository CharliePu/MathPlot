#include "Transformable.h"
#include <glm/ext/matrix_transform.hpp>

Transformable::Transformable() : transMat(1.0), netScale(1.0)
{
}

void Transformable::move(double x, double y)
{
	transMat = glm::translate(transMat, glm::vec3(x / netScale, y / netScale, 0));
}

void Transformable::zoom(double centerX, double centerY, double scale)
{
	netScale *= scale;
	transMat = glm::translate(glm::scale(glm::translate(transMat, glm::vec3(centerX, centerY, 0)), glm::vec3(scale, scale, 1)), glm::vec3(-centerX, -centerY, 0));
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

