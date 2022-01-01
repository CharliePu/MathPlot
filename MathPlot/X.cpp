#include "X.h"

std::unique_ptr<Expression> X::clone()
{
    return std::make_unique<X>();
}

double X::evaluate(double x, double y)
{
    return x;
}

std::string X::getString()
{
    return "x";
}
