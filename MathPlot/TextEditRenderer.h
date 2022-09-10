#pragma once

#include "TextEdit.h"
#include "LabelRenderer.h"

class TextEditRenderer: public Subscriber
{
public:
	TextEditRenderer();

	void draw();

	void addTextEdit(const std::shared_ptr<TextEdit>& textEdit);
	void removeTextEdit(const std::shared_ptr<TextEdit>& textEdit);

	void update(const std::shared_ptr<Publisher>& textEdit) override;
	void update();

	std::shared_ptr<TextEdit> within(double x, double y);
private:

	std::unordered_map<std::shared_ptr<Label>, std::shared_ptr<TextEdit>> labelParentInfos;

	std::shared_ptr<LabelRenderer> labelRenderer;
};

