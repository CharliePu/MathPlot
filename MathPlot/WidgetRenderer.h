#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Pane.h"
#include "Label.h"
#include "Font.h"

class WidgetRenderer
{
public:
	WidgetRenderer();

	void addWidget(const std::shared_ptr<Pane> &pane);
	void addWidget(const std::shared_ptr<Label>& label);
	void removeWidget(const std::shared_ptr<Widget>& widget);


	void render();

private:
	void rebuildVertices();

	Shader paneShader, labelShader;
	std::unique_ptr<Font> labelFont;

	std::unordered_set<std::shared_ptr<Widget>> panesCache, labelsCache;
	unsigned int panesVao, panesVbo, labelsVao, labelsVbo;
	size_t panesVerticesCount, labelsVerticesCount;
};

