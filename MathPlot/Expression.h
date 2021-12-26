#pragma once
#include <string>

class Expression
{
public:
	virtual double evaluate(double x, double y) = 0;
	virtual std::string getString() = 0;
};

