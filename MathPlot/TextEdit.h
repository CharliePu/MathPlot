#pragma once
#include "Label.h"
#include "Widget.h"

class TextEdit :
    public Label
{
public:
	TextEdit(double x, double y, const std::string& defaultText);

	void clickCallback() override;
	void typeCallback(char c) override;
	void typeUnfocusCallback() override;

private:
	bool isEditing;
	std::string defaultText;
};

