#include "Exponent.h"
#include <cmath>
#include <iostream>

Exponent::Exponent() = default;

Exponent::Exponent(std::unique_ptr<Expression> && l, std::unique_ptr<Expression> && r) :
    Operator(std::move(l), std::move(r))
{

}

std::unique_ptr<Expression> Exponent::clone()
{
    auto newExponent = std::make_unique<Exponent>();
    auto expressions = cloneExpressions();
    newExponent->setFirstExpression(std::move(expressions[0]));
    newExponent->setSecondExpression(std::move(expressions[1]));
    return std::move(newExponent);
}

double Exponent::evaluate(double x, double y)
{
    return std::pow(getFirstExpression()->evaluate(x, y), getSecondExpression()->evaluate(x, y));
}

Interval Exponent::evaluateInterval(const Interval& xi, const Interval& yi)
{
    // Using optimized functions
    if (auto c = getSecondExpression()->getConstant().value_or(0.0))
    {
        double intPart;
        if (std::modf(c, &intPart) == 0.0)
        {
            if (c == 2.0)
            {
                return boost::numeric::square(getFirstExpression()->evaluateInterval(xi, yi));
            }
            else if (c == -1.0)
            {
                return multiplicative_inverse(getFirstExpression()->evaluateInterval(xi, yi));
            }
            else
            {
                return boost::numeric::pow(getFirstExpression()->evaluateInterval(xi, yi), static_cast<int>(c));
            }
        }
        if (c > 0 && c < 1)
        {
            if (c == 0.5)
            {
                std::cout << "Optimized sqrt is used" << std::endl;
                return boost::numeric::sqrt(getFirstExpression()->evaluateInterval(xi, yi));
            }
            else
            {
                std::cout << "Optimized nth_root  is used" << std::endl;
                return boost::numeric::nth_root(getFirstExpression()->evaluateInterval(xi, yi), static_cast<int>(1.0 / c));
            }
        }
    }
    return boost::numeric::exp(getFirstExpression()->evaluateInterval(xi, yi) * boost::numeric::log(getSecondExpression()->evaluateInterval(xi, yi)));
}

std::string Exponent::getString()
{
	return getFirstExpression()->getString() + " ^ " + getSecondExpression()->getString();
}

std::optional<double> Exponent::getConstant()
{
    const auto c1 = getFirstExpression()->getConstant();
    const auto c2 = getSecondExpression()->getConstant();
    if (c1.has_value() && c2.has_value())
    {
        return std::pow(c1.value(), c2.value());
    }
    else
    {
        return std::nullopt;
    }
}
