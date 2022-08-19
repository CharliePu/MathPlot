#include "Controller.h"

#include <iostream>
#include <format>

#include "StatementParser.h"

Controller::Controller(Program& program): program(program), equationLabel(-0.95, -0.95, "Press I to plot an equation"),
											rangeLabel(-0.95, 0.9, "x: [-10, 10]; y: [-10, 10]"),
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
		equationLabel.draw();
		rangeLabel.draw();
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
		equationLabel.setText(plot.getStatement().getString());
	}
	else
	{
		equationLabel.setText("Invalid equation");
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
	equationLabel.update(width, height);
	rangeLabel.update(width, height);
}

void Controller::processRasterizerData()
{
	regionRenderer.resetTransform();
	regionRenderer.setPixels(rasterizer.getData(), rasterizer.getWidth(), rasterizer.getHeight());
}

void Controller::onMouseClicked()
{
	if (equationLabel.isClicked(program.getMouseX(), program.getMouseY()))
	{
		std::cout << "equationLabel clicked!" << std::endl;
	}
}

void Controller::onMouseDragged()
{
	auto scaleByNumericRange = [](const double val, const double min1, const double max1, const double min2, const double max2)
	{
		return val / (max1 - min1) * (max2 - min2);
	};
	
	plot.move(
		scaleByNumericRange(program.getMouseDeltaX(), -1, 1, plot.getXMin(), plot.getXMax()),
		scaleByNumericRange(program.getMouseDeltaY(), -1, 1, plot.getYMin(), plot.getYMax()));

	regionRenderer.move(program.getMouseDeltaX(), program.getMouseDeltaY());

	rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());

	gridRenderer.updatePlot(plot);

	rangeLabel.setText(std::format("x: [{:.2f}, {:.2f}]; y: [{:.2f}, {:.2f}]", plot.getXMin(), plot.getXMax(), plot.getYMin(), plot.getYMax()));
}

void Controller::onMouseScrolled()
{

	const double x = program.getMouseX();
	const double y = program.getMouseY();
	const double scale = pow(1.1, program.getMouseScroll());

	plot.zoom(x, y, scale);

	regionRenderer.zoom(x, y, scale);

	rasterizer.requestRasterize(plot, program.getWidth(), program.getHeight());

	gridRenderer.updatePlot(plot);

	rangeLabel.setText(std::format("x: [{:.2f}, {:.2f}]; y: [{:.2f}, {:.2f}]", plot.getXMin(), plot.getXMax(), plot.getYMin(), plot.getYMax()));
}
