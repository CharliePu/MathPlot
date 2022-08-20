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
	Interval base = getFirstExpression()->evaluateInterval(xi, yi);
    Interval exponent = getSecondExpression()->evaluateInterval(xi, yi);

    auto isWholeNumber = [](double x, int tolerance = 1)
    {
        return std::abs(std::round(x) - x) <= std::numeric_limits<double>::epsilon() * tolerance;
    };

    auto isConstant = [](Interval i)
    {
        return width(i) <= std::numeric_limits<double>::epsilon();
    };

    auto areEqual = [](double x, double y)
    {
        return std::abs(x - y) <= std::numeric_limits<double>::epsilon();
    };
    
    if (in_zero(base))
    {
        return Interval(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    }
    
    if (isConstant(exponent))
    {
        if (base.upper() < 0)
        {
            if (isWholeNumber(exponent.lower()))
            {
                const int exponentConstant = static_cast<int>(std::round(exponent.lower()));

                if (exponentConstant == 2)
                {
                    return square(base);
                }

                if (exponentConstant == -1)
                {
                    return multiplicative_inverse(base);
                }

                return pow(base, exponentConstant);
            }

            if (areEqual(exponent.lower(), 0.5))
            {
                return sqrt(base);
            }
        }
        
        if (exponent.lower() > 0 && exponent.lower() < 1 && isWholeNumber(1.0 / exponent.lower(), 2))
        {
            auto temp = boost::numeric::nth_root(base, static_cast<int>(std::round(1.0 / exponent.lower())));
            return temp;
        }
    }
    
    return boost::numeric::exp(exponent * boost::numeric::log(base));
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
