#pragma once
#include <vector>
#include "Shader.h"

class Renderer
{
public:
	static constexpr char vertexShaderPath[] = R"(.\shaders\shader.vert)";
	static constexpr char fragmentShaderPath[] = R"(.\shaders\shader.frag)";
	Renderer(int width, int height);


	void draw();
	void updateData(const std::vector<std::vector<int>> &data);
private:
	Shader shader;
	unsigned int vao, vbo, fbo;
};

