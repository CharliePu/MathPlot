#include "Constant.h"
#include <sstream>

Constant::Constant(double c): c(c)
{
}

double Constant::evaluate(double x, double y)
{
    return c;
}

std::string Constant::getString()
{
    std::ostringstream ss;
    ss << c;
    return ss.str();
}