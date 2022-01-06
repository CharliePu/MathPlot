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

boost::numeric::interval<double> Exponent::evaluateInterval(const boost::numeric::interval<double>& xi, const boost::numeric::interval<double>& yi)
{
    return boost::numeric::interval<double>(
        std::pow(getFirstExpression()->evaluateInterval(xi, yi).lower(), getSecondExpression()->evaluateInterval(xi, yi).lower()),
        std::pow(getFirstExpression()->evaluateInterval(xi, yi).upper(), getSecondExpression()->evaluateInterval(xi, yi).upper()));
}

std::string Exponent::getString()
{
	return getFirstExpression()->getString() + " ^ " + getSecondExpression()->getString();
}
