#pragma once
#include <string>

class Expression
{
public:
	virtual std::unique_ptr<Expression> clone() = 0;
	virtual double evaluate(double x, double y) = 0;
	virtual std::string getString() = 0;

};

