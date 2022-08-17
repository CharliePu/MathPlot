#pragma once
#include "Label.h"
#include "Widget.h"

class TextEdit :
    public Label
{
public:
	void clickCallback() override;
	bool isClicked(double mouseX, double mouseY) override;
	void typeCallback(char c) override;
	void typeUnfocusCallback() override;
	void setPosition(double x, double y) override;
	void update(int windowWidth, int windowHeight) override;
};

