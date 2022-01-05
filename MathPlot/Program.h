#pragma once
#include <functional>
#include <unordered_map>

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
	
	bool keyPressed(char c);

	bool mouseDragged();
	double getMouseDeltaX();
	double getMouseDeltaY();

	bool mouseScrolled();
	double getMouseScroll();

	int getWidth();
	int getHeight();

	void setOnWindowSizeChange(const std::function<void(int, int)>& f);

	bool shouldClose();
private:
	std::unordered_map<char, bool> keyState;
	double prevMouseX, prevMouseY, mouseX, mouseY;

	GLFWwindow* window;
};

Program& getProgram();
