#include "X.h"

std::unique_ptr<Expression> X::clone()
{
    return std::make_unique<X>();
}

double X::evaluate(double x, double y)
{
    return x;
}

Interval X::evaluateInterval(const Interval& xi, const Interval& yi)
{
    return xi;
}

std::string X::getString()
{
    return "x";
}
