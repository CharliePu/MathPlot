#include "Font.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <bit>

Font::Font(const FT_Face& face)
{
	constexpr unsigned int characterSize = 128;

	unsigned int charsPerRow = std::ceil(std::sqrt(characterSize));
	width = (1 + face->size->metrics.height / 64) * charsPerRow;
	width = std::bit_ceil(width); // Round up to nearest power of 2
	height = width;

	bitmap.resize(height * width);

	int penX = 0, penY = 0;

	for (int c = 0; c != characterSize; ++c)
	{
		if (penX + face->glyph->bitmap.width >= width)
		{
			penX = 0;
			penY += (1 + face->size->metrics.height / 64);
		}

		FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);

		for (int row = 0; row != face->glyph->bitmap.rows; ++row)
		{
			for (int col = 0; col != face->glyph->bitmap.width; ++col)
			{
				int x = penX + col;
				int y = penY + row;
				bitmap[y * width + x] = face->glyph->bitmap.buffer[row * face->glyph->bitmap.pitch + col];
			}
		}

		glyphInfos[c].offsetX = face->glyph->bitmap_left;
		glyphInfos[c].offsetY = face->glyph->bitmap_top;
		glyphInfos[c].advance = face->glyph->advance.x / 64;

		glyphInfos[c].width = face->glyph->bitmap.width;
		glyphInfos[c].height = face->glyph->bitmap.rows;

		glyphInfos[c].x0 = penX  / static_cast<double>(width);
		glyphInfos[c].y0 = penY / static_cast<double>(height);


		glyphInfos[c].x1 = (penX + glyphInfos[c].width) / static_cast<double>(width);
		glyphInfos[c].y1 = (penY + glyphInfos[c].height) / static_cast<double>(height);

		penX += face->glyph->bitmap.width + 1;
	}

	FT_Done_Face(face);
}

std::vector<unsigned char> Font::getBitmap()
{
	return bitmap;
}

unsigned int Font::getBitmapWidth()
{
	return width;
}

unsigned int Font::getBitmapHeight()
{
	return height;
}

std::unordered_map<char, GlyphInfo> Font::getGlyphInfos()
{
	return glyphInfos;
}

void Font::exportBitmap(const std::string& path)
{
	std::vector<unsigned char> data;
	data.resize(width * height * 4);

	for (int i = 0; i != width * height; ++i)
	{
		data[i * 4] = data[i * 4 + 1] = data[i * 4 + 2] = bitmap[i];
		data[i * 4 + 3] = 0xff;
	}

	stbi_write_png(path.c_str(), width, height, 4, data.data(), 4 * width);
}

std::unique_ptr<Font> createFont(const std::string& path, double size)
{
	static FontFactory factory{};
	return factory.createFont(path, size);
}

FontFactory::FontFactory()
{
	if (FT_Init_FreeType(&library))
	{
		std::cerr << "FT_Init_FreeType failed" << std::endl;
		return;
	}
}

FontFactory::~FontFactory()
{
	FT_Done_FreeType(library);
}

std::unique_ptr<Font> FontFactory::createFont(const std::string& path, unsigned int pixelHeight)
{
	FT_Face face;
	FT_New_Face(library, path.c_str(), 0, &face);
	FT_Set_Pixel_Sizes(face, 0, pixelHeight);
	return std::make_unique<Font>(face);
}