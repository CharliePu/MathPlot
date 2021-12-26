#include "Subtract.h"

Subtract::Subtract() = default;

Subtract::Subtract(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) :
    Operator(std::move(l), std::move(r))
{

}

double Subtract::evaluate(double x, double y)
{
    return getFirstExpression()->evaluate(x, y) - getSecondExpression()->evaluate(x, y);
}

int Subtract::getPrecedence()
{
    return 1;
}

std::string Subtract::getString()
{
    return getFirstExpression()->getString() + " - " + getSecondExpression()->getString();
}
