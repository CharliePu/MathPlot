#include "WidgetRenderer.h"


WidgetRenderer::WidgetRenderer(): 
	paneShader("./shaders/pane.vert", "./shaders/pane.frag"), 
	labelShader("./shaders/label.vert", "./shaders/label.frag"),
	labelFont(FontFactory().createFont("./fonts/arial.ttf", 64))
{
}

void WidgetRenderer::render()
{
	paneShader.use();
	glBindVertexArray(panesVao);
	glDrawArrays(GL_TRIANGLES, 0, panesVerticesCount);

	labelShader.use();
	glBindVertexArray(labelsVao);
}
