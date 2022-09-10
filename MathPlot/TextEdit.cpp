#include "TextEdit.h"


TextEdit::TextEdit(double x, double y, const std::string& defaultText): 
	label(std::make_shared<Label>(x, y, defaultText)), 
	editState(false), 
	defaultText(defaultText)
{
}

void TextEdit::setText(const std::string& text)
{
	label->setText(text);
}

std::string TextEdit::getText()
{
	return label->getText();
}

void TextEdit::setPosition(double x, double y)
{
	label->setPosition(x, y);
}

double TextEdit::getX()
{
	return label->getX();
}

double TextEdit::getY()
{
	return label->getY();
}

void TextEdit::setEditState(bool state)
{
	editState = state;
}

bool TextEdit::getEditState()
{
	return editState;
}

void TextEdit::setDefaultText(const std::string& text)
{
	defaultText = text;
}

std::string TextEdit::getDefaultText()
{
	return defaultText;
}

std::shared_ptr<Label> TextEdit::getLabel()
{
	return label;
}

void TextEdit::setLabel(const std::shared_ptr<Label>& label)
{
	this->label = label;
}
