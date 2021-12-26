#include "ExpressionParser.h"

#include <string>

#include "Constant.h"
#include "X.h"
#include "Y.h"
#include "Plus.h"
#include "Subtract.h"
#include "Divide.h"
#include "Multiply.h"

std::unique_ptr<Expression> ExpressionParser::parse(const std::string& exp)
{
	if (exp.empty())
	{
		return nullptr;
	}

	operatorStack = std::stack<std::unique_ptr<Operator>>();
	valueStack = std::stack<std::unique_ptr<Expression>>();

	char lastValidChar = ' ';

	for (int i = 0; i < exp.size();)
	{
		char c = exp[i];

		if (c >= '0' && c <= '9')
		{
			valueStack.push(std::make_unique<Constant>(extractNumber(exp, i)));
			lastValidChar = c;
			continue;
		}

		if (c == 'x' || c == 'y')
		{
			if ((lastValidChar >= '0' && lastValidChar <= '9') || (lastValidChar == 'x' || lastValidChar == 'y' && lastValidChar != c))
			{
				operatorStack.push(std::make_unique<Multiply>());
			}

			valueStack.push(createUnkown(c));
			lastValidChar = c;
		}

		if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			lastValidChar = c;
			auto op = createOperator(c);
			bool shouldProcess = !operatorStack.empty() && op->getPrecedence() < operatorStack.top()->getPrecedence();
			if (shouldProcess)
			{
				if (!process())
				{
					return nullptr;
				}
			}
			operatorStack.push(std::move(op));
		}
		i++;
	}

	if (lastValidChar == '+' || lastValidChar == '-' || lastValidChar == '*' || lastValidChar == '/')
	{
		return nullptr;
	}

	while (!operatorStack.empty())
	{
		if (!process())
		{
			return nullptr;
		}
	}

	if (valueStack.size() > 1)
	{
		return nullptr;
	}

	return std::move(valueStack.top());
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

bool ExpressionParser::process()
{
	if (operatorStack.size() < 1 || valueStack.size() < 2)
	{
		return false;
	}

	auto op = std::move(operatorStack.top());
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
