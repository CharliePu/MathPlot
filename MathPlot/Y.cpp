#include "Y.h"

std::unique_ptr<Expression> Y::clone()
{
    return std::make_unique<Y>();
}

double Y::evaluate(double x, double y)
{
    return y;
}

std::string Y::getString()
{
    return "y";
}
