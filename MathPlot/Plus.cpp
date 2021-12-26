#include "Plus.h"

Plus::Plus() = default;

Plus::Plus(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) :
    Operator(std::move(l), std::move(r))
{

}

double Plus::evaluate(double x, double y)
{
    return getFirstExpression()->evaluate(x, y) + getSecondExpression()->evaluate(x, y);
}

int Plus::getPrecedence()
{
    return 1;
}

std::string Plus::getString()
{
    return getFirstExpression()->getString() + " + " + getSecondExpression()->getString();
}
