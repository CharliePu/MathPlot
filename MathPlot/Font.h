#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "ft2build.h"
#include FT_FREETYPE_H

struct GlyphInfo
{
	unsigned int width, height, advance;
	int offsetX, offsetY;
	double x0, y0, x1, y1;
};

class Font
{
public:
	Font(const FT_Face &face);

	void loadGlyphInfo(const unsigned int& characterSize, const FT_Face& face);

	std::vector<unsigned char> getBitmap();
	[[nodiscard]] unsigned int getBitmapWidth() const;
	[[nodiscard]] unsigned int getBitmapHeight() const;
	std::unordered_map<char, GlyphInfo> getGlyphInfos();

	void exportBitmap(const std::string& path) const;
private:
	std::unordered_map<char, GlyphInfo> glyphInfos;
	std::vector<unsigned char> bitmap;
	unsigned int width, height;
};

class FontFactory
{
public:
	FontFactory();
	~FontFactory();

	[[nodiscard]] std::unique_ptr<Font> createFont(const std::string& path, unsigned int pixelHeight) const;
private:
	FT_Library library;
};

std::unique_ptr<Font> createFont(const std::string &path, double size);