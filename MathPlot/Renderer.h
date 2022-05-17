#pragma once
#include <vector>

#include "Drawable.h"
#include "Shader.h"


class Renderer: public Drawable
{
public:
	void draw() override;

	void move(double x, double y);
	void zoom(double x, double y, double s);
	void resetTransform();

private:

};

