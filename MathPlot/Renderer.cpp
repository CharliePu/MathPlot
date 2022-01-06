#include "Renderer.h"
#include <glm/ext/matrix_transform.hpp>

Renderer::Renderer(): transMat(1.0), netScale(1.0)
{
}

void Renderer::move(double x, double y)
{
	transMat = glm::translate(transMat, glm::vec3(x / netScale, y / netScale, 0));
}

void Renderer::zoom(double x, double y, double s)
{
	double scale = pow(1.1, s);
	netScale *= scale;
	transMat = glm::translate(glm::scale(glm::translate(transMat, glm::vec3(x, y, 0)), glm::vec3(scale, scale, 1)), glm::vec3(-x, -y, 0));
}

void Renderer::resetTransform()
{
	transMat = glm::identity<glm::mat4>();
	netScale = 1.0;
}

glm::mat4 Renderer::getTransMat()
{
	return transMat;
}
