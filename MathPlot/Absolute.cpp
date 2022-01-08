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

Interval Absolute::evaluateInterval(const Interval& xi, const Interval& yi)
{
    return boost::numeric::abs(getFirstExpression()->evaluateInterval(xi, yi));
}

std::string Absolute::getString()
{
    return "|" + getFirstExpression()->getString() + "|";
}

std::optional<double> Absolute::getConstant()
{
	if (const auto c = getFirstExpression()->getConstant(); c.has_value())
    {
        return std::abs(c.value());
    }
    else
    {
        return std::nullopt;
    }
}
