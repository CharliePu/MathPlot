#include "Constant.h"
#include <sstream>

Constant::Constant(double c): c(c)
{
}

std::unique_ptr<Expression> Constant::clone()
{
    return std::make_unique<Constant>(c);
}


double Constant::evaluate(double x, double y)
{
    return c;
}

boost::numeric::interval<double> Constant::evaluateInterval(const boost::numeric::interval<double>& xi, const boost::numeric::interval<double>& yi)
{
    return boost::numeric::interval<double>(c);
}

std::string Constant::getString()
{
    std::ostringstream ss;
    ss << c;
    return ss.str();
}