#pragma once
#include <memory>
#include <string>
#include <stack>

#include "Operator.h"
#include "Value.h"


class ExpressionParser
{
public:
	std::unique_ptr<Expression> parse(std::string exp);
private:
	double extractNumber(const std::string& exp, int& i);
	bool isNumber(char c);
	bool isOperator(char c);
	bool isUnkown(char c);
	int getOperatorPrecedence(char c);


	bool process();

	std::stack<char> operatorStack;
	std::stack<std::unique_ptr<Expression>> valueStack;
};

