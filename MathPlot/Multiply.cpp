#include "Multiply.h"

Multiply::Multiply() = default;

Multiply::Multiply(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) :
    Operator(std::move(l), std::move(r))
{

}

std::unique_ptr<Expression> Multiply::clone()
{
    auto newMultiply = std::make_unique<Multiply>();
    auto expressions = cloneExpressions();
    newMultiply->setFirstExpression(std::move(expressions[0]));
    newMultiply->setSecondExpression(std::move(expressions[1]));
    return std::move(newMultiply);
}

double Multiply::evaluate(double x, double y)
{
    return getFirstExpression()->evaluate(x, y) * getSecondExpression()->evaluate(x, y);
}

Interval Multiply::evaluateInterval(const Interval& xi, const Interval& yi)
{
    return getFirstExpression()->evaluateInterval(xi, yi) * getSecondExpression()->evaluateInterval(xi, yi);
}

std::string Multiply::getString()
{
    return getFirstExpression()->getString() + " * " + getSecondExpression()->getString();
}

std::optional<double> Multiply::getConstant()
{
    const auto c1 = getFirstExpression()->getConstant();
    const auto c2 = getSecondExpression()->getConstant();
    if (c1.has_value() && c2.has_value())
    {
        return c1.value() * c2.value();
    }
    else
    {
        return std::nullopt;
    }
}
