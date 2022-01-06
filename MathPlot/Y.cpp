#include "Y.h"

std::unique_ptr<Expression> Y::clone()
{
    return std::make_unique<Y>();
}

double Y::evaluate(double x, double y)
{
    return y;
}

boost::numeric::interval<double> Y::evaluateInterval(const boost::numeric::interval<double>& xi, const boost::numeric::interval<double>& yi)
{
    return yi;
}

std::string Y::getString()
{
    return "y";
}
