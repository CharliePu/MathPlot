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

Interval Constant::evaluateInterval(const Interval& xi, const Interval& yi)
{
    return Interval(c);
}

std::string Constant::getString()
{
    std::ostringstream ss;
    ss << c;
    return ss.str();
}

std::optional<double> Constant::getConstant()
{
    return c;
}
