#pragma once
class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;

	Drawable(const Drawable& other) = delete;
	Drawable& operator=(const Drawable& other) = delete;
	Drawable(Drawable&& other) = delete;
	Drawable& operator=(Drawable&& other) = delete;

	virtual void draw() = 0;
};

