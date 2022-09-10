#pragma once
#include "Widget.h"

#include <string>

#include "glm/mat4x4.hpp"

#include "Publisher.h"


class Label: public Publisher
{
public:
    
    Label(double x, double y, const std::string& text);

	void setText(const std::string& text);
	std::string getText();

    void setPosition(double x, double y);

    double getX();
    double getY();

private:
    std::string text;
    double x, y;
};

