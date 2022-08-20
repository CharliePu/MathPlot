#pragma once
#include "GridRenderer.h"
#include "Label.h"
#include "Program.h"
#include "Rasterizer.h"
#include "RegionRenderer.h"
#include "TextEdit.h"

class Controller
{
public:
	explicit Controller(Program &program);
	~Controller();

	Controller(const Controller& controller) = delete;
	Controller& operator=(const Controller& controller) = delete;
	Controller(Controller&& controller) = delete;
	Controller& operator=(Controller&& controller) = delete;

	void start();

private:
	void consoleInputTask();
	void startConsoleInput();
	void stopConsoleInput();
	void processEquationInput(const std::string& inputString);
	void requestConsoleInput();

	void onWindowSizeChange(int width, int height);

	void processRasterizerData();

	void onMouseClicked();
	void onMouseDragged();
	void onMouseScrolled();

	Program &program;

	Rasterizer rasterizer;
	RegionRenderer regionRenderer;

	GridRenderer gridRenderer;

	Label rangeLabel;
	TextEdit equationTextEdit;
	
	Plot plot;

	std::thread consoleInputThread;
	std::atomic<bool> consoleInputThreadShouldClose, consoleInputDataReady;
	bool consoleInputRequested;
	std::condition_variable consoleInputRequestedCv;
	std::mutex consoleInputRequestedMutex;
	std::string consoleInputString;

	bool isTypingEquation;
};
