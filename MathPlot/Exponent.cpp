#include "Exponent.h"
#include <cmath>

Exponent::Exponent() = default;

Exponent::Exponent(std::unique_ptr<Expression> && l, std::unique_ptr<Expression> && r) :
    Operator(std::move(l), std::move(r))
{

}

std::unique_ptr<Expression> Exponent::clone()
{
    auto newExponent = std::make_unique<Exponent>();
    auto expressions = cloneExpressions();
    newExponent->setFirstExpression(std::move(expressions[0]));
    newExponent->setSecondExpression(std::move(expressions[1]));
    return std::move(newExponent);
}

double Exponent::evaluate(double x, double y)
{
    return std::pow(getFirstExpression()->evaluate(x, y), getSecondExpression()->evaluate(x, y));
}

std::string Exponent::getString()
{
	return getFirstExpression()->getString() + " ^ " + getSecondExpression()->getString();
}
