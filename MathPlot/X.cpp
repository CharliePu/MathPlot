#include "X.h"

std::unique_ptr<Expression> X::clone()
{
    return std::make_unique<X>();
}

double X::evaluate(double x, double y)
{
    return x;
}

boost::numeric::interval<double> X::evaluateInterval(const boost::numeric::interval<double>& xi, const boost::numeric::interval<double>& yi)
{
    return xi;
}

std::string X::getString()
{
    return "x";
}
