#include "TextEditController.h"

void TextEditController::clickCallback(const std::shared_ptr<TextEdit>& textEdit)
{
	textEdit->setEditState(true);
	textEdit->setText("");
	textEdit->notifySubscribers();
}

void TextEditController::typeCallback(const std::shared_ptr<TextEdit> & textEdit, char c)
{
	if (textEdit->getEditState()) 
	{
		if (c == '\b')
		{
			std::string s{ textEdit->getText() };
			if (!s.empty())
				s.pop_back();
			textEdit->setText(s);
		}
		else
		{
			textEdit->setText(textEdit->getText() + c);
		}
		textEdit->notifySubscribers();
	}
}

void TextEditController::typeUnfocusCallback(const std::shared_ptr<TextEdit>& textEdit)
{
	textEdit->setEditState(false);

	if (textEdit->getText().empty())
	{
		textEdit->setText(textEdit->getDefaultText());
	}
	textEdit->notifySubscribers();
}
