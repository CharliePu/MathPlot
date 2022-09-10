#pragma once
#include "Label.h"
#include "Widget.h"

class TextEdit: public Publisher
{
public:
	TextEdit(double x, double y, const std::string& defaultText);

	void setText(const std::string& text);
	std::string getText();

	void setPosition(double x, double y);
	double getX();
	double getY();

	void setEditState(bool state);
	bool getEditState();

	void setDefaultText(const std::string &text);
	std::string getDefaultText();

	std::shared_ptr<Label> getLabel();
	void setLabel(const std::shared_ptr<Label> &label);

private:
	bool editState;
	std::string defaultText;
	std::shared_ptr<Label> label;
};

