#include "Absolute.h"
#include <cmath>

Absolute::Absolute() = default;

Absolute::Absolute(std::unique_ptr<Expression> && e) :
    Operator(std::move(e))
{

}

std::unique_ptr<Expression> Absolute::clone()
{
    auto newAbsolute = std::make_unique<Absolute>();
    auto expressions = cloneExpressions();
    newAbsolute->setFirstExpression(std::move(expressions[0]));
    return std::move(newAbsolute);
}

double Absolute::evaluate(double x, double y)
{
    return std::abs(getFirstExpression()->evaluate(x, y));
}

std::string Absolute::getString()
{
    return "|" + getFirstExpression()->getString() + "|";
}
