#include "Label.h"



void Label::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

Label::Label(double x, double y, const std::string& text)
{
	Label::setPosition(x, y);
	setText(text);
}

void Label::setText(const std::string& text)
{
	this->text = text;
}

std::string Label::getText()
{
	return text;
}

double Label::getX()
{
	return x;
}

double Label::getY()
{
	return y;
}
