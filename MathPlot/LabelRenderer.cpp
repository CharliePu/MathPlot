#include "LabelRenderer.h"

#include <glm/gtc/type_ptr.hpp>

LabelRenderer::LabelRenderer():
	font(FontFactory().createFont("./fonts/arial.ttf", 64)),
	shader(R"(.\shaders\label.vert)", R"(.\shaders\label.frag)")
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (void*)(2 * sizeof(double)));
	

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font->getBitmapWidth(), font->getBitmapHeight(), NULL, GL_RED,
		GL_UNSIGNED_BYTE, font->getBitmap().data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

std::shared_ptr<Label> LabelRenderer::within(double x, double y)
{
	for (auto labelInfo : labelInfos)
	{
		bool isWithin = 
			x > labelInfo.second.boundX0 && 
			x < labelInfo.second.boundX1&&
			y > labelInfo.second.boundY0 &&
			y < labelInfo.second.boundY1;

		if (isWithin)
		{
			return labelInfo.first;
		}
	}

	return nullptr;
}

void LabelRenderer::addLabel(const std::shared_ptr<Label>& label)
{
	label->subscribe(shared_from_this());
	generateVertices(label);
	sendVertices();
}

void LabelRenderer::removeLabel(const std::shared_ptr<Label>& label)
{
	label->unsubscribe(shared_from_this());
	labelInfos.erase(label);
	sendVertices();
}

void LabelRenderer::draw()
{
	shader.use();
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


void LabelRenderer::sendVertices()
{
	std::vector<double> vertices{};
	for (auto& labelInfo : labelInfos)
	{
		vertices.insert(
			vertices.end(), 
			labelInfo.second.vertices.begin(), 
			labelInfo.second.vertices.end());
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
}

void LabelRenderer::update(const std::shared_ptr<Publisher>& l)
{
	auto label = std::dynamic_pointer_cast<Label>(l);
	if (labelInfos.find(label) != labelInfos.end())
	{
		generateVertices(label);
		sendVertices();
	}
}

void LabelRenderer::update()
{
	for (auto& labelInfo : labelInfos)
	{
		generateVertices(labelInfo.first);
	}

	sendVertices();
}

void LabelRenderer::generateVertices(const std::shared_ptr<Label>& label)
{
	using std::min;
	using std::max;

	auto glyphInfos = font->getGlyphInfos();
	auto program = getProgram();
	auto windowWidth = program->getWidth();
	auto windowHeight = program->getHeight();

	auto& vertices = labelInfos[label].vertices;
	vertices.clear();

	double penX = 0, penY = 0;
	for (char c : label->getText())
	{
		double posX1 = (penX + glyphInfos[c].offsetX) / windowWidth * windowHeight;
		double posY1 = penY + glyphInfos[c].offsetY;
		double texX1 = glyphInfos[c].x0;
		double texY1 = glyphInfos[c].y1;

		double posX0 = (penX + glyphInfos[c].offsetX + glyphInfos[c].width) / windowWidth * windowHeight;
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

	// Scale down the text
	for (int i = 0; i != vertices.size(); ++i)
	{
		if (i % 4 < 2)
		{
			vertices[i] /= 1024.0;
			if (i % 2 == 0)
			{
				vertices[i] += label->getX();
			}
			else
			{
				vertices[i] += label->getY();
			}
		}
	}

	// Update bounds for click detection
	auto boundX0 = std::numeric_limits<double>::max();
	auto boundX1 = std::numeric_limits<double>::lowest();
	auto boundY0 = std::numeric_limits<double>::max();
	auto boundY1 = std::numeric_limits<double>::lowest();
	for (int i = 0; i != vertices.size();)
	{
		boundX0 = min(boundX0, vertices[i]);
		boundX1 = max(boundX1, vertices[i]);
		boundY0 = min(boundY0, vertices[i + 1]);
		boundY1 = max(boundY1, vertices[i + 1]);
		i += 4;
	}

	const glm::vec4 transformedBound0 = transformMat * glm::vec4(boundX0, boundY0, 0, 1);
	const glm::vec4 transformedBound1 = transformMat * glm::vec4(boundX1, boundY1, 0, 1);

	labelInfos[label].boundX0 = transformedBound0.x;
	labelInfos[label].boundY0 = transformedBound0.y;
	labelInfos[label].boundX1 = transformedBound1.x;
	labelInfos[label].boundY1 = transformedBound1.y;
}
