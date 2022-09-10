#include "Controller.h"

#include <iostream>
#include <format>

#include "StatementParser.h"

Controller::Controller(const std::shared_ptr<Program> &program): 
	program(program), 
	equationTextEdit(std::make_shared<TextEdit>(-0.95, -0.95, "Click here and type an inequality")),
	rangeLabel(std::make_shared<Label>(- 0.95, 0.9, "x: [-10, 10]; y: [-10, 10]")),
	labelController(std::make_shared<LabelController>()),
	textEditController(std::make_shared<TextEditController>()),
	labelRenderer(std::make_shared<LabelRenderer>()),
	textEditRenderer(std::make_shared<TextEditRenderer>()),
    consoleInputRequested(false), isTypingEquation(false)
{
	program->setOnWindowSizeChange([&](const int width, const int height) { onWindowSizeChange(width, height); });



	textEditRenderer->addTextEdit(equationTextEdit);
	labelRenderer->addLabel(rangeLabel);

	startConsoleInput();
}

Controller::~Controller()
{
	stopConsoleInput();
}

void Controller::start()
{
	while (!program->shouldClose())
	{
		if (rasterizer.isDataReady())
		{
			processRasterizerData();
		}
		
		if (consoleInputDataReady)
		{
			processEquationInput(consoleInputString);
		}

		if (program->mouseDragged())
		{
			onMouseDragged();
		}

		if (program->mouseClicked())
		{
			onMouseClicked();
		}

		if (program->mouseScrolled())
		{
			onMouseScrolled();
		}

		if (isTypingEquation)
		{
			if (program->hasTextInput())
			{
				textEditController->typeCallback(equationTextEdit, program->getTextInput());
			}

			// Special handling for removing characters
			if (program->backSpacePressed())
			{
				textEditController->typeCallback(equationTextEdit, '\b');
			}

			if (program->enterPressed() || program->escapePressed())
			{
				textEditController->typeUnfocusCallback(equationTextEdit);
				isTypingEquation = false;
				processEquationInput(equationTextEdit->getText());
			}
		}
		else
		{
			if (program->keyPressed('I'))
			{
				requestConsoleInput();
			}

			if (program->keyPressed('D'))
			{
				rasterizer.toggleDebug();
			}
		}

		gridRenderer.draw();
		regionRenderer.draw();
		textEditRenderer->draw();
		labelRenderer->draw();
		
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

void Controller::processEquationInput(const std::string &inputString)
{
	plot.setStatement(StatementParser().parse(inputString));
	consoleInputDataReady = false;
	if (!plot.empty())
	{
		rasterizer.requestRasterize(plot, program->getWidth(), program->getHeight());
		equationTextEdit->setText(plot.getStatement().getString());
	}
	else
	{
		equationTextEdit->setText("Invalid equation");
		equationTextEdit->notifySubscribers();
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

	textEditRenderer->update();
	labelRenderer->update();
}

void Controller::processRasterizerData()
{
	regionRenderer.resetTransform();
	regionRenderer.setPixels(rasterizer.getData(), rasterizer.getWidth(), rasterizer.getHeight());
}

void Controller::onMouseClicked()
{
	if (!isTypingEquation)
	{
		if (textEditRenderer->within(program->getMouseX(), program->getMouseY()) == equationTextEdit)
		{
			textEditController->clickCallback(equationTextEdit);
			isTypingEquation = true;
		}
	}
}

void Controller::onMouseDragged()
{
	auto scaleByNumericRange = [](const double val, const double min1, const double max1, const double min2, const double max2)
	{
		return val / (max1 - min1) * (max2 - min2);
	};
	
	plot.move(
		scaleByNumericRange(program->getMouseDeltaX(), -1, 1, plot.getXMin(), plot.getXMax()),
		scaleByNumericRange(program->getMouseDeltaY(), -1, 1, plot.getYMin(), plot.getYMax()));

	regionRenderer.move(program->getMouseDeltaX(), program->getMouseDeltaY());

	rasterizer.requestRasterize(plot, program->getWidth(), program->getHeight());

	gridRenderer.updatePlot(plot);

	rangeLabel->setText(std::format("x: [{:.2f}, {:.2f}]; y: [{:.2f}, {:.2f}]", plot.getXMin(), plot.getXMax(), plot.getYMin(), plot.getYMax()));
}

void Controller::onMouseScrolled()
{

	const double x = program->getMouseX();
	const double y = program->getMouseY();
	const double scale = pow(1.1, program->getMouseScroll());

	plot.zoom(x, y, scale);

	regionRenderer.zoom(x, y, scale);

	rasterizer.requestRasterize(plot, program->getWidth(), program->getHeight());

	gridRenderer.updatePlot(plot);

	rangeLabel->setText(std::format("x: [{:.2f}, {:.2f}]; y: [{:.2f}, {:.2f}]", plot.getXMin(), plot.getXMax(), plot.getYMin(), plot.getYMax()));
}
