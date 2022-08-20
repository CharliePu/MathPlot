#include "TextEdit.h"


TextEdit::TextEdit(double x, double y, const std::string& defaultText): Label(x, y, defaultText), isEditing(false), defaultText(defaultText)
{
}

void TextEdit::clickCallback()
{
	isEditing = true;
	setText("");
}

void TextEdit::typeCallback(char c)
{
	if (isEditing) {
		if (c == '\b')
		{
			std::string s{ getText() };
			if (!s.empty())
				s.pop_back();
			setText(s);
		}
		else
		{
			setText(getText() + c);
		}
	}
}

void TextEdit::typeUnfocusCallback()
{
	isEditing = false;

	if (getText().empty())
	{
		setText(defaultText);
	}
}
