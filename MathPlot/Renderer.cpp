#include "Renderer.h"
#include <glad/glad.h>

Renderer::Renderer()
{
}

void Renderer::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::updateData(const std::vector<std::vector<int>>& data)
{
	
}
