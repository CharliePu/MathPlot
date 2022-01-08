#include "Divide.h"

Divide::Divide()
{
}

Divide::Divide(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) :
    Operator(std::move(l), std::move(r))
{

}

std::unique_ptr<Expression> Divide::clone()
{
    auto newDivide = std::make_unique<Divide>();
    auto expressions = cloneExpressions();
    newDivide->setFirstExpression(std::move(expressions[0]));
    newDivide->setSecondExpression(std::move(expressions[1]));
    return std::move(newDivide);
}

double Divide::evaluate(double x, double y)
{
    return getFirstExpression()->evaluate(x, y) / getSecondExpression()->evaluate(x, y);
}

Interval Divide::evaluateInterval(const Interval& xi, const Interval& yi)
{
    return getFirstExpression()->evaluateInterval(xi, yi) / getSecondExpression()->evaluateInterval(xi, yi);
}

std::string Divide::getString()
{
    return getFirstExpression()->getString() + " / " + getSecondExpression()->getString();
}

std::optional<double> Divide::getConstant()
{
	const auto c1 = getFirstExpression()->getConstant();
	const auto c2 = getSecondExpression()->getConstant();
    if (c1.has_value() && c2.has_value())
    {
        return c1.value() / c2.value();
    }
    else
    {
        return std::nullopt;
    }
}
