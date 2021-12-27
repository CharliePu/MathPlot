#pragma once
#include <vector>
//#include "Shader.h"

class Renderer
{
public:
	Renderer();


	void draw();
	void updateData(const std::vector<std::vector<int>> &data);
private:
	//Shader shader;
};

