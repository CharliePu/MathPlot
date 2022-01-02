#include "ExpressionParser.h"

#include <string>

#include "Constant.h"
#include "X.h"
#include "Y.h"
#include "Plus.h"
#include "Subtract.h"
#include "Divide.h"
#include "Multiply.h"

std::unique_ptr<Expression> ExpressionParser::parse(std::string exp)
{
	// remove spaces
	exp.erase(std::remove(exp.begin(), exp.end(), ' '), exp.end());

	if (exp.empty())
	{
		return nullptr;
	}

	operatorStack = std::stack<char>();
	valueStack = std::stack<std::unique_ptr<Expression>>();


	char prevValidChar = ' ';

	for (int i = 0; i < exp.size();)
	{
		char c = exp[i];

		if (isNumber(c))
		{
			// y2 => y * 2 is illegal
			if (isUnkown(prevValidChar))
			{
				return nullptr;
			}

			double number = extractNumber(exp, i);
			valueStack.push(std::make_unique<Constant>(number));
			prevValidChar = c;
		}
		else if (isUnkown(c))
		{
			// xy => x * y and 2x => 2*x
			if (isNumber(prevValidChar) || isUnkown(prevValidChar))
			{
				// xx => x*x or yy => y*y is illegal
				if (prevValidChar == c)
				{
					return nullptr;
				}

				operatorStack.push('*');
			}

			valueStack.push(createUnkown(c));
			prevValidChar = c;
			i++;

		}
		else if (isOperator(c))
		{
			while (!operatorStack.empty() && getOperatorPrecedence(c) < getOperatorPrecedence(operatorStack.top()))
			{
				if (!process())
				{
					return nullptr;
				}
			}
			operatorStack.push(c);
			prevValidChar = c;
			i++;
		}
		else if (c == '(')
		{
			// 3(x+y) =>3*(x+y)
			if (isUnkown(prevValidChar) || isNumber(prevValidChar) || prevValidChar == ')')
			{
				operatorStack.push('*');
			}

			operatorStack.push(c);
			prevValidChar = c;
			i++;
		}
		else if (c == ')')
		{
			while (!operatorStack.empty() && operatorStack.top() != '(')
			{
				if (!process())
				{
					return nullptr;
				}
			}

			if (operatorStack.empty())
			{
				return nullptr;
			}
			else
			{
				operatorStack.pop();
			}

			prevValidChar = c;
			i++;
		}
		else
		{
			return nullptr;
		}
	}

	if (isOperator(prevValidChar))
	{
		return nullptr;
	}

	while (!operatorStack.empty())
	{
		if (operatorStack.top() == ')')
		{
			while (!operatorStack.empty() && operatorStack.top() != '(')
			{
				if (!process())
				{
					return nullptr;
				}
			}

			if (operatorStack.empty())
			{
				return nullptr;
			}
			else
			{
				operatorStack.pop();
			}
		}
		else if (!process())
		{
			return nullptr;
		}
	}

	if (valueStack.size() == 1)
	{
		return std::move(valueStack.top());
	}
	else
	{
		return nullptr;
	}
}

double ExpressionParser::extractNumber(const std::string& exp, int& i)
{
	if ((i > exp.size() || exp[i] < '0' || exp[i] > '9') && exp[i] != '.')
	{
		return 0;
	}

	int j = 0;
	bool dot = false;
	while (i + j < exp.size() && exp[i + j] >= '0' && exp[i + j] <= '9' || exp[i + j] == '.')
	{
		if (exp[i + j] == '.') {
			if (!dot)
			{
				dot = true;
			}
			else
			{
				break;
			}
		}
		j++;
	}
	
	std::string s = exp.substr(i, j);
	double d = std::stod(s);

	i += j;

	return d;
}

bool ExpressionParser::isNumber(char c)
{
	return c >= '0' && c <= '9';
}

bool ExpressionParser::isOperator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool ExpressionParser::isUnkown(char c)
{
	return c == 'x' || c == 'y';
}

int ExpressionParser::getOperatorPrecedence(char c)
{
	switch (c)
	{
	case '+':
	case '-':
		return 2;
	case '*':
	case '/':
		return 3;
	case '^':
		return 5;
	case '(':
		return 1;
	default:
		return 0;
	}
}


bool ExpressionParser::process()
{
	if (operatorStack.size() < 1 || valueStack.size() < 2)
	{
		return false;
	}

	auto op = std::move(createOperator(operatorStack.top()));
	operatorStack.pop();
	
	auto rv = std::move(valueStack.top());
	valueStack.pop();

	auto lv = std::move(valueStack.top());
	valueStack.pop();

	op->setFirstExpression(std::move(lv));
	op->setSecondExpression(std::move(rv));
	valueStack.push(std::move(op));

	return true;
}
