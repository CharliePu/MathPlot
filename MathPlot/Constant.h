#pragma once
#include "Value.h"
class Constant :
    public Value
{
public:
    Constant(double c);
    std::unique_ptr<Expression> clone() override;
    double evaluate(double x, double y) override;
    Interval evaluateInterval(const Interval& xi, const Interval& yi) override;
    std::string getString() override;
    std::optional<double> getConstant() override;
private:
    double c;
};

