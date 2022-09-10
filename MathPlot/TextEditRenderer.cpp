#include "TextEditRenderer.h"

TextEditRenderer::TextEditRenderer(): 
	labelRenderer(std::make_shared<LabelRenderer>())
{
}

void TextEditRenderer::draw()
{
	labelRenderer->draw();
}

void TextEditRenderer::addTextEdit(const std::shared_ptr<TextEdit>& textEdit)
{
	textEdit->subscribe(shared_from_this());
	labelRenderer->addLabel(textEdit->getLabel());
	labelParentInfos[textEdit->getLabel()] = textEdit;
}

void TextEditRenderer::removeTextEdit(const std::shared_ptr<TextEdit>& textEdit)
{
	textEdit->unsubscribe(shared_from_this());
	labelRenderer->removeLabel(textEdit->getLabel());
	labelParentInfos.erase(textEdit->getLabel());
}

void TextEditRenderer::update(const std::shared_ptr<Publisher>& textEdit)
{
	labelRenderer->update(std::dynamic_pointer_cast<TextEdit>(textEdit)->getLabel());
}

void TextEditRenderer::update()
{
	labelRenderer->update();
}

std::shared_ptr<TextEdit> TextEditRenderer::within(double x, double y)
{
	if (auto label = labelRenderer->within(x, y))
	{
		return labelParentInfos[label];
	}
	return nullptr;
}