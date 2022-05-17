#pragma once
#include "Operator.h"
class Absolute :
    public Operator
{
public:
    Absolute();
    Absolute(std::unique_ptr<Expression>&& e);
    std::unique_ptr<Expression> clone() override;
    double evaluate(double x, double y) override;
    Interval evaluateInterval(const Interval& xi, const Interval& yi) override;
    std::string getString() override;
    std::optional<double> getConstant() override;
};

