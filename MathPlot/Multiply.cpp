#include "Multiply.h"

Multiply::Multiply() = default;

Multiply::Multiply(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) :
    Operator(std::move(l), std::move(r))
{

}

double Multiply::evaluate(double x, double y)
{
    return getFirstExpression()->evaluate(x, y) * getSecondExpression()->evaluate(x, y);
}

int Multiply::getPrecedence()
{
    return 2;
}

std::string Multiply::getString()
{
    return getFirstExpression()->getString() + " * " + getSecondExpression()->getString();
}
