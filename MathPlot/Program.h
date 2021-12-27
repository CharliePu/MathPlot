#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Program
{
public:
	static constexpr int screenWidth = 800;
	static constexpr int screenHeight = 800;
	static constexpr char title[] = "MathPlot";

	Program();
	~Program();
	
	bool shouldClose();
private:
	GLFWwindow* window;
};

Program& getProgram();
