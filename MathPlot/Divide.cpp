#include "Divide.h"

Divide::Divide()
{
}

Divide::Divide(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r) :
    Operator(std::move(l), std::move(r))
{

}

double Divide::evaluate(double x, double y)
{
    return getFirstExpression()->evaluate(x, y) / getSecondExpression()->evaluate(x, y);
}

int Divide::getPrecedence()
{
    return 2;
}

std::string Divide::getString()
{
    return getFirstExpression()->getString() + " / " + getSecondExpression()->getString();
}
