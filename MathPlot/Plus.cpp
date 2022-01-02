#include "Plus.h"

Plus::Plus() = default;

Plus::Plus(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) :
    Operator(std::move(l), std::move(r))
{

}

std::unique_ptr<Expression> Plus::clone()
{
    auto newPlus = std::make_unique<Plus>();
    auto expressions = cloneExpressions();
    newPlus->setFirstExpression(std::move(expressions[0]));
    newPlus->setSecondExpression(std::move(expressions[1]));
    return std::move(newPlus);
}

double Plus::evaluate(double x, double y)
{
    return getFirstExpression()->evaluate(x, y) + getSecondExpression()->evaluate(x, y);
}

std::string Plus::getString()
{
    return getFirstExpression()->getString() + " + " + getSecondExpression()->getString();
}
