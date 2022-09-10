#pragma once
#include <memory>
#include "TextEdit.h"

class TextEditController
{
public:
	void clickCallback(const std::shared_ptr<TextEdit> & textEdit);
	void typeCallback(const std::shared_ptr<TextEdit> & textEdit, char c);
	void typeUnfocusCallback(const std::shared_ptr<TextEdit> & textEdit);
};