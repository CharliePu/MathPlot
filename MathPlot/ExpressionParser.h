#pragma once
#include <memory>
#include <string>
#include <stack>

#include "Operator.h"
#include "Value.h"


class ExpressionParser
{
public:
	std::unique_ptr<Expression> parse(const std::string& exp);
private:
	double extractNumber(const std::string& exp, int& i);
	bool process();

	std::stack<std::unique_ptr<Operator>> operatorStack;
	std::stack<std::unique_ptr<Expression>> valueStack;
};

