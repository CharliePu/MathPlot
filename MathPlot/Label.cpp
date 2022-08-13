#include "Label.h"

#include <glm/gtc/type_ptr.hpp>

void Label::clickCallback()
{
}

bool Label::isClicked(double mouseX, double mouseY)
{
	return false;
}

void Label::typeCallback(char c)
{
}

void Label::typeUnfocusCallback()
{
}

void Label::draw()
{
	shader.use();
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindVertexArray(vao);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMat));
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Label::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;

	transformMat = glm::translate(glm::mat4(1.0), glm::vec3(x, y, 0));
}

void Label::update(int windowWidth, int windowHeight)
{
	this->windowRatio = windowWidth / static_cast<double>(windowHeight);
	generateVertices();
}

Label::Label(): font(FontFactory().createFont("./fonts/arial.ttf", 64)),
                shader(R"(.\shaders\label.vert)", R"(.\shaders\label.frag)"),
				windowRatio(1.0)
{
}

Label::Label(double x, double y, const std::string& text) : Label()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (void *)(2 * sizeof(double)));

	transformLoc = glGetUniformLocation(shader, "transMat");

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font->getBitmapWidth(), font->getBitmapHeight(), NULL, GL_RED, GL_UNSIGNED_BYTE, font->getBitmap().data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	Label::setPosition(x, y);
	setText(text);
}

void Label::generateVertices()
{
	auto glyphInfos = font->getGlyphInfos();

	vertices.clear();
	vertices.reserve(text.size() * 6 * 4);

	double penX = 0, penY = 0;
	for (char c : text)
	{
		double posX1 = (penX + glyphInfos[c].offsetX) / windowRatio;
		double posY1 = penY + glyphInfos[c].offsetY;
		double texX1 = glyphInfos[c].x0;
		double texY1 = glyphInfos[c].y1;

		double posX0 = (penX + glyphInfos[c].offsetX + glyphInfos[c].width) / windowRatio;
		double posY0 = penY + glyphInfos[c].offsetY - glyphInfos[c].height;
		double texX0 = glyphInfos[c].x1;
		double texY0 = glyphInfos[c].y0;

		vertices.push_back(posX1);
		vertices.push_back(posY0);
		vertices.push_back(texX1);
		vertices.push_back(texY1);

		vertices.push_back(posX0);
		vertices.push_back(posY0);
		vertices.push_back(texX0);
		vertices.push_back(texY1);

		vertices.push_back(posX1);
		vertices.push_back(posY1);
		vertices.push_back(texX1);
		vertices.push_back(texY0);

		vertices.push_back(posX1);
		vertices.push_back(posY1);
		vertices.push_back(texX1);
		vertices.push_back(texY0);

		vertices.push_back(posX0);
		vertices.push_back(posY1);
		vertices.push_back(texX0);
		vertices.push_back(texY0);

		vertices.push_back(posX0);
		vertices.push_back(posY0);
		vertices.push_back(texX0);
		vertices.push_back(texY1);

		penX += glyphInfos[c].advance;
	}

	vertexCount = vertices.size() / 4.0;

	for (int i = 0; i != vertices.size(); ++i)
	{
		if (i % 4 < 2)
		{
			vertices[i] /= 1024.0;
		}
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
}

void Label::setText(const std::string& text)
{
	this->text = text;
	generateVertices();
}
