#pragma once
#include <functional>


struct GLFWwindow;

class Program
{
public:
	static constexpr int screenWidth = 800;
	static constexpr int screenHeight = 800;
	static constexpr char title[] = "MathPlot";

	Program();
	~Program();
	
	bool keyPressing(char c);
	bool keyPressed(char c);

	bool mouseClicked();
	bool mouseDragged();
	[[nodiscard]] double getMouseX() const;
	[[nodiscard]] double getMouseY() const;
	[[nodiscard]] double getMouseDeltaX() const;
	[[nodiscard]] double getMouseDeltaY() const;

	bool mouseScrolled();
	double getMouseScroll();

	[[nodiscard]] int getWidth() const;
	[[nodiscard]] int getHeight() const;

	void setOnWindowSizeChange(const std::function<void(int, int)>& f);

	[[nodiscard]] bool shouldClose() const;
private:
	double prevMouseX, prevMouseY, mouseX, mouseY;

	GLFWwindow* window;
};

Program& getProgram();
