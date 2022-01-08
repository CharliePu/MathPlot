#include "Subtract.h"

Subtract::Subtract() = default;

Subtract::Subtract(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) :
    Operator(std::move(l), std::move(r))
{

}

std::unique_ptr<Expression> Subtract::clone()
{
    auto newSubtract = std::make_unique<Subtract>();
    auto expressions = cloneExpressions();
    newSubtract->setFirstExpression(std::move(expressions[0]));
    newSubtract->setSecondExpression(std::move(expressions[1]));
    return std::move(newSubtract);
}

double Subtract::evaluate(double x, double y)
{
    return getFirstExpression()->evaluate(x, y) - getSecondExpression()->evaluate(x, y);
}

Interval Subtract::evaluateInterval(const Interval& xi, const Interval& yi)
{
    return getFirstExpression()->evaluateInterval(xi, yi) - getSecondExpression()->evaluateInterval(xi, yi);
}

std::string Subtract::getString()
{
    return getFirstExpression()->getString() + " - " + getSecondExpression()->getString();
}
