#pragma once
class Widget
{
public:

	virtual void clickCallback() = 0;
	virtual bool within(double x, double y) = 0;

	virtual void typeCallback(char c) = 0;
	virtual void typeUnfocusCallback() = 0;

	virtual void draw() = 0;
	virtual void setPosition(double x, double y) = 0;
	
	virtual void update(int windowWidth, int windowHeight) = 0;
};

