#include "Controller.h"

#include <iostream>

#include "StatementParser.h"

Controller::Controller(Program& program): program(program), label(-0.95, -0.95, "Press I to plot an equation"),
                                          consoleInputRequested(false)
{
	program.setOnWindowSizeChange([&](const int width, const int height) { onWindowSizeChange(width, height); });

	startConsoleInput();
}

Controller::~Controller()
{
	stopConsoleInput();
}

void Controller::start()
{
	while (!program.shouldClose())
	{

		if (rasterizer.isDataReady())
		{
			processRasterizerData();
		}
		
		if (consoleInputDataReady)
		{
			processConsoleInput();
		}

		if (program.keyPressed('I'))
		{
			requestConsoleInput();
		}
		if (program.keyPressed('D'))
		{
			rasterizer.toggleDebug();
		}

		if (program.mouseDragged())
		{
			onMouseDragged();
		}

		if (program.mouseClicked())
		{
			onMouseClicked();
		}

		if (program.mouseScrolled())
		{
			onMouseScrolled();
		}

		gridRenderer.draw();
		regionRenderer.draw();
		label.draw();
	}
}

void Controller::consoleInputTask()
{
	while (!consoleInputThreadShouldClose)
	{
		std::unique_lock inputRequestedLock(consoleInputRequestedMutex);
		consoleInputRequestedCv.wait(inputRequestedLock, [&]
		{
			return (consoleInputRequested && !consoleInputDataReady);
		});
		inputRequestedLock.unlock();

		if (!consoleInputThreadShouldClose)
		{
			std::cout << "Input an equation/inequality: ";
			std::cin >> consoleInputString;
			consoleInputDataReady = true;
			inputRequestedLock.lock();
			consoleInputRequested = false;
			inputRequestedLock.unlock();
		}
	}
}

void Controller::startConsoleInput()
{
	consoleInputThreadShouldClose = false;
	consoleInputThread = std::thread([&] { consoleInputTask(); });
}

void Controller::stopConsoleInput()
{
	consoleInputThreadShouldClose = true;
	consoleInputDataReady = false;
	std::unique_lock<std::mutex> inputRequestedLock(consoleInputRequestedMutex);
	consoleInputRequested = true;
	inputRequestedLock.unlock();
	consoleInputRequestedCv.notify_one();
	consoleInputThread.join();
}

void Controller::processConsoleInput()
{
	plot.setStatement(StatementParser().parse(consoleInputString));
	consoleInputDataReady = false;
	if (!plot.empty())
	{
		rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());
		label.setText(plot.getStatement().getString());
	}
	else
	{
		label.setText("Invalid equation");
	}
}

void Controller::requestConsoleInput()
{
	std::unique_lock<std::mutex> inputRequestLock(consoleInputRequestedMutex);
	consoleInputRequested = true;
	inputRequestLock.unlock();
	consoleInputRequestedCv.notify_one();
}

void Controller::onWindowSizeChange(int width, int height)
{
	if (!plot.empty())
	{
		if (width > 0 && height > 0)
		{
			plot.setAspectRatio(width / static_cast<double>(height));
			rasterizer.requestRasterize(plot, width, height);
		}
	}
	label.update(width, height);
}

void Controller::processRasterizerData()
{
	regionRenderer.resetTransform();
	regionRenderer.setPixels(rasterizer.getData(), rasterizer.getWidth(), rasterizer.getHeight());
}

void Controller::onMouseClicked()
{
	if (label.isClicked(program.getMouseX(), program.getMouseY()))
	{
		std::cout << "label clicked!" << std::endl;
	}
}

void Controller::onMouseDragged()
{
	plot.move(program.getMouseDeltaX() * (plot.getXMax() - plot.getXMin()),
	          program.getMouseDeltaY() * (plot.getYMax() - plot.getYMin()));

	regionRenderer.move(program.getMouseDeltaX() * 2, -program.getMouseDeltaY() * 2);

	rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());

	gridRenderer.updatePlot(plot);
}

void Controller::onMouseScrolled()
{
	plot.zoom(plot.getXMin() + program.getMouseX() * (plot.getXMax() - plot.getXMin()),
	          plot.getYMax() - program.getMouseY() * (plot.getYMax() - plot.getYMin()),
	          -program.getMouseScroll());

	regionRenderer.zoom(program.getMouseX() * 2 - 1, -program.getMouseY() * 2 + 1, program.getMouseScroll());
	rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());

	gridRenderer.updatePlot(plot);
}
